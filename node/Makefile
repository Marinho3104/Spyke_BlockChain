
root_path := ./../
own_path := ./

all_compilable_files := $(own_path)*.cpp
all_object_files := $(own_path)*.o
all_header_files := $(own_path)*.h

headers: # Copies all headers into the root path

	cp $(all_header_files) $(root_path)

compile_object_files: # Compile all files into object files

	# Move files into root path 
	cp $(all_compilable_files) $(root_path)

	# Compile into object files
	g++ $(FLAG) -c -g $(root_path)*.cpp

	# Move all object files into root path
	mv $(all_object_files) $(root_path)

	# Remove files
	$(MAKE) clean

clean:

	rm -f $(root_path)*.cpp


