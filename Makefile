
own_path := ./

all_object_files := $(own_path)*.o

wallet_settings_generator_name := wallet_settings_generator.out
node_settings_generator_name := node_settings_generator.out
output_name := output.out

memory_pool_path := $(own_path)memory_pool
blockchain_path := $(own_path)blockchain
consensus_path := $(own_path)consensus
wallet_path := $(own_path)wallet
crypto_path := $(own_path)crypto
miner_path := $(own_path)miner
utils_path := $(own_path)utils
types_path := $(own_path)types
p2p_path := $(own_path)p2p
node_path := $(own_path)node

#Flags
wallet_test_compilation_flag := FLAG=-DWALLET_TEST=
miner_test_compilation_flag := FLAG=-DMINER_TEST=
node_test_compilation_flag := FLAG=-DNODE_TEST=

# P2P tests directory
p2p_tests_f := $(own_path)node_tests/f
p2p_tests_s := $(own_path)node_tests/s

# Wallet tests directory
wallet_tests := $(own_path)wallet_tests/

# Miner tests directory
miner_tests_f := $(own_path)miner_tests/f
miner_tests_s := $(own_path)miner_tests/s

run:

	$(MAKE) miner_test || $(MAKE) clean
 
compile_object_files_g++:

	g++ -g $(all_object_files) -o $(output_name)

compile_object_files_nvcc:

	nvcc -rdc=true -arch=sm_70 -Wno-deprecated-gpu-targets -g $(all_object_files) -o $(output_name)

headers:

	$(MAKE) -C $(memory_pool_path) headers
	$(MAKE) -C $(blockchain_path) headers
	$(MAKE) -C $(consensus_path) headers
	$(MAKE) -C $(wallet_path) headers
	$(MAKE) -C $(crypto_path) headers
	$(MAKE) -C $(utils_path) headers
	$(MAKE) -C $(miner_path) headers
	$(MAKE) -C $(types_path) headers
	$(MAKE) -C $(node_path) headers
	$(MAKE) -C $(p2p_path) headers

wallet_test:

	# Copy the headers
	$(MAKE) headers

	$(MAKE) -C $(wallet_path) $(wallet_test_compilation_flag) compile_object_files 
	$(MAKE) -C $(memory_pool_path) compile_object_files
	$(MAKE) -C $(blockchain_path) compile_object_files
	$(MAKE) -C $(crypto_path) compile_object_files
	$(MAKE) -C $(utils_path) compile_object_files
	$(MAKE) -C $(types_path) compile_object_files
	$(MAKE) -C $(p2p_path) compile_object_files

	$(MAKE) compile_object_files_nvcc

	# Remove files
	$(MAKE) clean

	mv $(own_path)*.out $(wallet_tests)

node_test:

	# Copy the headers
	$(MAKE) headers

	$(MAKE) -C $(node_path) $(node_test_compilation_flag) compile_object_files
	$(MAKE) -C $(memory_pool_path) compile_object_files
	$(MAKE) -C $(blockchain_path) compile_object_files
	$(MAKE) -C $(wallet_path) compile_object_files 
	$(MAKE) -C $(crypto_path) compile_object_files
	$(MAKE) -C $(utils_path) compile_object_files
	$(MAKE) -C $(types_path) compile_object_files
	$(MAKE) -C $(p2p_path) compile_object_files

	$(MAKE) compile_object_files_nvcc

	# Remove files
	$(MAKE) clean

	cp $(own_path)*.out $(p2p_tests_f)
	mv $(own_path)*.out $(p2p_tests_s)

node_settings_generator_test:

	$(MAKE) -C $(node_path) compile_node_settings_generator

	g++ -g $(all_object_files) -o $(node_settings_generator_name)

	# Remove files
	$(MAKE) clean

	cp $(own_path)*.out $(p2p_tests_f)
	mv $(own_path)*.out $(p2p_tests_s)

wallet_settings_generator_test:

	$(MAKE) headers

	$(MAKE) -C $(blockchain_path) compile_object_files
	$(MAKE) -C $(crypto_path) compile_object_files
	$(MAKE) -C $(wallet_path) compile_wallet_settings_generator

	g++ -g $(all_object_files) -o $(wallet_settings_generator_name)

	# Remove files
	$(MAKE) clean

	mv $(own_path)*.out $(wallet_tests)

miner_test:

	$(MAKE) headers

	$(MAKE) -C $(miner_path) $(miner_test_compilation_flag) compile_object_files
	$(MAKE) -C $(memory_pool_path) compile_object_files
	$(MAKE) -C $(blockchain_path) compile_object_files
	$(MAKE) -C $(consensus_path) compile_object_files 
	$(MAKE) -C $(wallet_path) compile_object_files 
	$(MAKE) -C $(crypto_path) compile_object_files
	$(MAKE) -C $(utils_path) compile_object_files
	$(MAKE) -C $(types_path) compile_object_files
	$(MAKE) -C $(node_path) compile_object_files
	$(MAKE) -C $(p2p_path) compile_object_files

	$(MAKE) compile_object_files_nvcc

	# Remove files
	$(MAKE) clean

	cp $(own_path)*.out $(miner_tests_f)
	mv $(own_path)*.out $(miner_tests_s)

clean:

	rm -f *.h *.o *.cu *.cuh *.cpp *.cp *.c