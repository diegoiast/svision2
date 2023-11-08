/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

enum class CheckboxShape {
    Checkbox,
    RadioButton,
};

struct ItemStatus {
    bool is_selected = false;
    bool is_active = false;
};
