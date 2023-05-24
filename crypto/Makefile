
root_path := ./../
own_path := ./

# Ed25519 
ed25519_path := $(own_path)ed25519/src/
gpu_fs_path = $(own_path)gpufs/libgpufs/

ed25519_all_compilable_files := $(ed25519_path)*.c

all_object_files := $(own_path)*.o

headers: # Copies all headers into root path

	# Ed25519 copy
	cp $(ed25519_path)*.h $(root_path)

compile_object_files: # Compile all files into object files

	# Ed25519 object files

	# Move files into root path 
	cp $(ed25519_all_compilable_files) $(root_path)

	# Compile into object files
	g++ -c -g $(root_path)*.c

	# Move all object files into root path
	mv $(all_object_files) $(root_path)

	# Remove files
	$(MAKE) clean

clean:

	rm -f $(root_path)*.c



