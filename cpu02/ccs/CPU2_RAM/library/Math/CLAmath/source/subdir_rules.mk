################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
library/Math/CLAmath/source/%.obj: ../library/Math/CLAmath/source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/ccs/library/App_lib/Controller/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/ccs/library/App_lib/Motor/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/ccs/library/App_lib/MPPT/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/ccs/library/App_lib/Observer/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/ccs/library/App_lib/Other/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/dual/sdfm_filters_sync_cla/cpu02/" --define=CPU2 -g --diag_suppress=10063 --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="library/Math/CLAmath/source/$(basename $(<F)).d_raw" --obj_directory="library/Math/CLAmath/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


