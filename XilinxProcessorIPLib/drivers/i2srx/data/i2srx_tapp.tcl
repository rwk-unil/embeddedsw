###############################################################################
# Copyright (C) 2017 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMANGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#
#
###############################################################################
# Uses $XILINX_EDK/bin/lib/xillib_sw.tcl
#
# - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - - - -
# Software Project Types (swproj):
# 0 : MemoryTest - Calls basic  memorytest routines from common driver dir
# 1 : PeripheralTest - Calls any existing polled_example and/or selftest
# - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - - - -
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# TCL Procedures:
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

proc gen_include_files {swproj mhsinst} {
	if {$swproj == 0} {
		return ""
  }
  if {$swproj == 1} {
	  set inc_file_lines {xi2srx_header.h}
	  return $inc_file_lines
  }
}

proc gen_src_files {swproj mhsinst} {
	if {$swproj == 0} {
		return ""
  }
  if {$swproj == 1} {
	  set inc_file_lines {examples/xi2srx_selftest_example.c data/xi2srx_header.h}
	  return $inc_file_lines
  }
}

proc gen_testfunc_def {swproj mhsinst} {
	return ""
}

proc gen_init_code {swproj mhsinst} {
	return ""
}

proc gen_testfunc_call {swproj mhsinst} {

	if {$swproj == 0} {
		return ""
  }

  set ipname [get_property NAME $mhsinst]
  set deviceid [::hsi::utils::get_ip_param_name $mhsinst "DEVICE_ID"]
  set stdout [get_property CONFIG.STDOUT [get_os]]
  if { $stdout == "" || $stdout == "none" } {
	  set hasStdout 0
   } else {
	   set hasStdout 1
   }

   set testfunc_call ""

   if {${hasStdout} == 0} {

	   append testfunc_call "

	   {
	   int Status;

	   Status = I2srx_SelfTest_Example(${deviceid});

   }"
  } else {

	  append testfunc_call "

	  {
	  int Status;


	  print(\"\\r\\nRunning I2srx_SelfTest_Example() for ${ipname}...\\r\\n\");

	  Status = I2srx_SelfTest_Example(${deviceid});

	  if (Status == 0) {
	  print(\"I2srx_SelfTest_Example PASSED\\r\\n\");
      }
	      else {
	      print(\"I2srx_SelfTest_Example FAILED\\r\\n\");
      }
   }"
  }
  return $testfunc_call
}
