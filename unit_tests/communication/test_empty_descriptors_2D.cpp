#include <communication/GCL.h>
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <communication/high-level/descriptors_dt.h>
#include <communication/high-level/descriptors.h>
#include <string>

#define DIM 10

struct pair_t {
  int x,y;
  pair_t(int a, int b): x(a), y(b) {}
  pair_t(): x(0), y(0) {}
};

std::ostream& operator<<(std::ostream &s, pair_t const & t) { 
  return s << " (" 
           << t.x << ", "
           << t.y << ") ";
}

bool operator==(pair_t const & a, pair_t const & b) {
  return (a.x == b.x && 
          a.y == b.y);
}

bool operator!=(pair_t const & a, pair_t const & b) {
  return !(a==b);
}

void printbuff(std::ostream &file, pair_t* a) {
  file << "------------\n";
  for (int ii=0; ii<10; ++ii) {
    file << "|";
    for (int jj=0; jj<10; ++jj) {
      file << a[ii*10+jj];
    }
    file << "|\n";
  }
  file << "------------\n\n";
}



int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  gridtools::GCL_Init(argc, argv);

  pair_t *a = new pair_t[DIM*DIM*DIM];
  pair_t *b = new pair_t[DIM*DIM*DIM];
  pair_t *c = new pair_t[DIM*DIM*DIM];

  for (int ii=0; ii<=DIM; ++ii)
    for (int jj=0; jj<=DIM; ++jj) {
      a[gridtools::access(jj,ii,DIM,DIM)] = pair_t(0,0);
      b[gridtools::access(jj,ii,DIM,DIM)] = pair_t(0,0);                                      
      c[gridtools::access(jj,ii,DIM,DIM)] = pair_t(0,0);
    }      

  int pid;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  int nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  std::cout << pid << " " << nprocs << "\n";

  std::stringstream ss;
  ss << pid;

  std::string filename = "out" + ss.str() + ".txt";

  std::cout << filename << std::endl;
  std::ofstream file(filename.c_str());

  file << pid << "  " << nprocs << "\n";

  MPI_Comm CartComm;
  int dims[2] = {0,0};
  MPI_Dims_create(nprocs, 2, dims);
  int period[2] = {0, 0};

  file << "@" << pid << "@ MPI GRID SIZE " << dims[0] << " - " << dims[1] << "\n";
 
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, false, &CartComm);

  typedef gridtools::_2D_process_grid_t<gridtools::boollist<2> > grid_type;

  gridtools::hndlr_dynamic_ut<pair_t,2, 
    gridtools::Halo_Exchange_2D<grid_type>
    > hd(gridtools::boollist<2>(false,false), nprocs, pid);

  hd.halo.add_halo(1, 2, 1, 3, 6, DIM);
  hd.halo.add_halo(0, 2, 1, 3, 6, DIM);

  hd.allocate_buffers(3);

  int pi, pj;
  hd.pattern().proc_grid().coords(pi, pj);
  int PI, PJ;
  hd.pattern().proc_grid().dims(PI, PJ);

  file << "Proc: (" << pi << ", " << pj << ")\n";


  for (int ii=3; ii<=6; ++ii)
    for (int jj=3; jj<=6; ++jj) {
      // if (pid==6)
      //   a[gridtools::access(jj,ii,DIM,DIM)] = pair_t(-(ii-3+4*pj),-(jj-3+4*pi));
      // else
        a[gridtools::access(jj,ii,DIM,DIM)] = pair_t(ii-3+4*pj,jj-3+4*pi);
    }      

  for (int ii=3; ii<=6; ++ii)
    for (int jj=3; jj<=6; ++jj) {
        b[gridtools::access(jj,ii,DIM,DIM)] = pair_t(ii-3+4*pj,jj-3+4*pi);
    }      

  for (int ii=3; ii<=6; ++ii)
    for (int jj=3; jj<=6; ++jj) {
        c[gridtools::access(jj,ii,DIM,DIM)] = pair_t(ii-3+4*pj,jj-3+4*pi);
    }      

  printbuff(file,a);
  printbuff(file,b);
  printbuff(file,c);

  hd.pack(a,b,c);

  hd.exchange();

  hd.unpack(a,b,c);

  file << "\n********************************************************************************\n";

  printbuff(file,a);
  printbuff(file,b);
  printbuff(file,c);

  // CHECK!
  bool err=false;
  for (int ii=3-((pj>0)?2:0); ii<=6+((pj<PJ-1)?1:0); ++ii)
    for (int jj=3-((pi>0)?2:0); jj<=6+((pi<PI-1)?1:0); ++jj) {
        if (a[gridtools::access(jj,ii,DIM,DIM)] != pair_t(ii-3+4*pj,jj-3+4*pi)) {
          err=true;
          file << " A " 
                    << ii << ", "
                    << jj << ", "
                    << a[gridtools::access(jj,ii,DIM,DIM)] << " != "
                    << pair_t(ii-2+4*pj,jj-2+4*pi) << "\n";
        }
    }

  for (int ii=3-((pj>0)?2:0); ii<=6+((pj<PJ-1)?1:0); ++ii)
    for (int jj=3-((pi>0)?2:0); jj<=6+((pi<PI-1)?1:0); ++jj) {
        if (b[gridtools::access(jj,ii,DIM,DIM)] != pair_t(ii-3+4*pj,jj-3+4*pi)) {
          err=true;
          file << " B "
                    << ii << ", "
                    << jj << ", "
                    << b[gridtools::access(jj,ii,DIM,DIM)] << " != "
                    << pair_t(ii-3+4*pj,jj-3+4*pi) << "\n";
        }
    }

  for (int ii=3-((pj>0)?2:0); ii<=6+((pj<PJ-1)?1:0); ++ii)
    for (int jj=3-((pi>0)?2:0); jj<=6+((pi<PI-1)?1:0); ++jj) {
        if (c[gridtools::access(jj,ii,DIM,DIM)] != pair_t(ii-3+4*pj,jj-3+4*pi)) {
          err=true;
          file << " C "
                    << ii << ", "
                    << jj << ", "
                    << c[gridtools::access(jj,ii,DIM,DIM)] << " != "
                    << pair_t(ii+7*pj,jj-3+4*pi) << "\n";
        }
    }


  std::cout << std::boolalpha << err << " (False is good)" << std::endl;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
