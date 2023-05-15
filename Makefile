
own_path := ./

all_object_files := $(own_path)*.o

output_name := output.out

blockchain_path := $(own_path)blockchain
wallet_path := $(own_path)wallet
crypto_path := $(own_path)crypto
utils_path := $(own_path)utils
types_path := $(own_path)types

#Flags
wallet_test_compilation_flag := FLAG=-DWALLET_TEST=

run:

	$(MAKE) wallet_test

compile_object_files_g++:

	g++ -g $(all_object_files) -o $(output_name)

headers:

	$(MAKE) -C $(blockchain_path) headers
	$(MAKE) -C $(wallet_path) headers
	$(MAKE) -C $(crypto_path) headers
	$(MAKE) -C $(utils_path) headers
	$(MAKE) -C $(types_path) headers

wallet_test:

	# Copy the headers
	$(MAKE) headers

	$(MAKE) -C $(wallet_path) $(wallet_test_compilation_flag) compile_object_files 
	$(MAKE) -C $(blockchain_path) compile_object_files
	$(MAKE) -C $(crypto_path) compile_object_files
	$(MAKE) -C $(utils_path) compile_object_files
	$(MAKE) -C $(types_path) compile_object_files

	$(MAKE) compile_object_files_g++

	# Remove files
	$(MAKE) clean


clean:

	rm -f *.h *.o