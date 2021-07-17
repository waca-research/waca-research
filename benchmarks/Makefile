#######################################################################################################
# Specify workload names and relative path
#######################################################################################################
app_basicmath       = Workload-01-Basicmath             ./mi-bench-default/basicmath
app_dijkstra        = Workload-02-Dijkstra              ./mi-bench-default/dijkstra
app_fft             = Workload-05-FFT                   ./mi-bench-default/fft
app_patricia        = Workload-06-Patricia              ./mi-bench-default/patricia
app_qsort           = Workload-07-Qsort                 ./mi-bench-default/qsort
app_sha             = Workload-08-SHA                   ./mi-bench-default/sha
app_stringsearch    = Workload-10-StringSearch          ./mi-bench-default/stringsearch
app_adpcm           = Workload-11-ADPCM                 ./mi-bench-default/adpcm
app_aes             = Workload-12-AES                   ./mi-bench-default/aes
app_kmeans          = Workload-13-Kmeans                ./cortexsuite-default/kmeans
app_spectral        = Workload-14-Spectral              ./cortexsuite-default/spectral
app_me              = Workload-15-MotionEstimation      ./cortexsuite-default/motion-estimation
app_pca             = Workload-16-PCA                   ./cortexsuite-default/pca
app_rbm             = Workload-17-RBM                   ./cortexsuite-default/rbm
app_blackscholes2t  = Workload-21-Blackscholes-2T       ./parsec-source-default/blackscholes
app_blackscholes4t  = Workload-22-Blackscholes-4T       ./parsec-source-default/blackscholes
app_blackscholes8t  = Workload-23-Blackscholes-8T       ./parsec-source-default/blackscholes
app_fluidanimate2t  = Workload-24-Fluidanimate-2T       ./parsec-source-default/fluidanimate
app_fluidanimate4t  = Workload-25-Fluidanimate-4T       ./parsec-source-default/fluidanimate
app_fluidanimate8t  = Workload-26-Fluidanimate-8T       ./parsec-source-default/fluidanimate
app_lenet5          = Workload-27-LeNet5                ./dnn-default/lenet5
app_vgg16_1t        = Workload-28-VGG16-1T              ./dnn-default/vgg16
app_vgg16_2t        = Workload-29-VGG16-2T              ./dnn-default/vgg16
app_vgg16_4t        = Workload-30-VGG16-4T              ./dnn-default/vgg16
app_vgg16_6t        = Workload-31-VGG16-6T              ./dnn-default/vgg16

#######################################################################################################
# Specify list of workloads to run
#######################################################################################################
workloads = $(app_basicmath) $(app_dijkstra) $(app_fft) $(app_patricia) $(app_qsort) \
            $(app_sha) $(app_stringsearch) $(app_adpcm) $(app_aes) \
            $(app_kmeans) $(app_spectral) $(app_me) $(app_pca) $(app_rbm) \
            $(app_blackscholes2t) $(app_blackscholes4t) $(app_blackscholes8t) \
            $(app_fluidanimate2t) $(app_fluidanimate4t) $(app_fluidanimate8t) \
            $(app_lenet5) \
            $(app_vgg16_1t) $(app_vgg16_2t) $(app_vgg16_4t) $(app_vgg16_6t)

#######################################################################################################
# Internals
#######################################################################################################
pwd = $(shell pwd)
COMMON_DIR = ./common

include $(COMMON_DIR)/common.mk 

#######################################################################################################
# Targets
#######################################################################################################
compile:
	$(COMMON_DIR)/run_script.sh compile $(PLATFORM) $(COMMON_DIR) $(ALGORITHM) $(GOVERNOR) $(workloads)

run:
	$(COMMON_DIR)/run_script.sh run $(PLATFORM) $(COMMON_DIR) $(ALGORITHM) $(GOVERNOR) $(workloads)

clean_binaries:
	rm -rf $(ALGORITHM)/*.o
	rm -rf $(ALGORITHM)/output*

clean_workloads:
	$(COMMON_DIR)/run_script.sh clean $(PLATFORM) $(COMMON_DIR) $(ALGORITHM) $(GOVERNOR) $(workloads)
	rm -rf $(ALGORITHM)/output*
