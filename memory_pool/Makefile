
root_path := ./../
own_path := ./

all_compilable_files_cuda := $(own_path)*.cu
all_compilable_files := $(own_path)*.cpp
all_object_files := $(own_path)*.o
all_header_files := $(own_path)*.h
all_header_files_cuda := $(own_path)*.cuh

headers: # Copies all headers into the root path

	cp $(all_header_files) $(root_path)
	cp $(all_header_files_cuda) $(root_path)

compile_object_files: # Compile all files into object files

	# Move files into root path 
	cp $(all_compilable_files) $(root_path)

	# Compile into object files
	g++ $(FLAG) -c -g $(root_path)*.cpp

	# Move all object files into root path
	mv $(all_object_files) $(root_path)


	# Move files into root path 
	cp $(all_compilable_files_cuda) $(root_path)

	# Compile into object files
	nvcc -rdc=true -arch=sm_70 -Wno-deprecated-gpu-targets -g -c $(root_path)*.cu

	# Move all object files into root path
	mv $(all_object_files) $(root_path)

	# Remove files
	$(MAKE) clean

clean:

	rm -f $(root_path)*.cpp


