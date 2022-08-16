# GT4Py Project - GridTools Framework
#
# Copyright (c) 2014-2021, ETH Zurich
# All rights reserved.
#
# This file is part of the GT4Py project and the GridTools framework.
# GT4Py is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or any later
# version. See the LICENSE.txt file at the top-level directory of this
# distribution for a copy of the license or check <https://www.gnu.org/licenses/>.
#
# SPDX-License-Identifier: GPL-3.0-or-later

from __future__ import annotations

import pathlib


_file = pathlib.Path(__file__)


def get_cmake_dir() -> pathlib.Path:
    return _file.parent / "data" / "lib" / "cmake" / "GridTools"


def get_include_dir() -> pathlib.Path:
    return _file.parent / "data" / "lib" / "cmake" / "GridTools"
