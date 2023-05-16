
root_path := ./../
own_path := ./

socket_path := $(own_path)socket/
socket_all_compilable_files := $(socket_path)*.cpp
socket_all_header_files := $(socket_path)*.h

protocols_path := $(own_path)protocols/
protocols_all_compilable_files := $(protocols_path)*.cpp
protocols_all_header_files := $(protocols_path)*.h

all_object_files := $(own_path)*.o

headers:

	# Copies socket headers
	cp $(socket_all_header_files) $(root_path)

	# Copies protocols headers
	cp $(protocols_all_header_files) $(root_path)

compile_object_files: # Compile all files into object files

	# Move socket files into root path 
	cp $(socket_all_compilable_files) $(root_path)

	# Move protocols files into root path 
	cp $(protocols_all_compilable_files) $(root_path)

	# Compile into object files
	g++ -c -g $(root_path)*.cpp

	# Move all object files into root path
	mv $(all_object_files) $(root_path)

	# Remove files
	$(MAKE) clean

clean:

	rm -f $(root_path)*.cpp


