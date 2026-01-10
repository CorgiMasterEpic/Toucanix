/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2025 CorgiMasterEpic (https://github.com/CorgiMasterEpic)
*/

#include <stdtypes.hpp>
#include <Shrimp/bootinfo.hpp>
#include <Citrus2D/citrus.hpp>
#include <Citrus2D/console.hpp>
#include <Kernel/kConfig.hpp>

class Kernel { // Codename: Corgi
public:
  static constexpr const char *CorgiInfo = "Toucanix 26.0.1 devloper preview";
  Kernel(BootInfo *bootInfo);
  void kMain();

private:
  BootInfo *bootInfo;
  Citrus citrus;
};
