/*
* DEEPCRAFT Studio 5.9.4563.0+34bdb7f4372a1120ca38a0cb02e62db5b4b78270
* Copyright © 2023- Imagimob AB, All Rights Reserved.
* 
* Generated at 02/22/2026 18:55:01 UTC. Any changes will be lost.
* 
* Model ID  715c8fac-1c23-45ce-9dbf-21f5875d9ce5
* 
* Memory    Size                      Efficiency
* Buffers   53748 bytes (RAM)         100 %
* State     51408 bytes (RAM)         100 %
* Readonly  96404 bytes (Flash)       100 %
* 
* Backend              tensorflow
* Keras Version        2.15.0
* Backend Model Type   Sequential
* Backend Model Name   conv1d-small-speed-3
* 
* Class Index | Symbol Label
* 0           | (unlabeled)
* 1           | empty_box
* 2           | table
* 
* Layer                          Shape           Type       Function
* Sliding Window (data points)   [100,128]       float      dequeue
*    window_shape = [100,128]
*    stride = 2560
*    buffer_multiplier = 1
* Contextual Window (Sliding Window) [100,128]       float      dequeue
*    contextual_length_sec = 0.5
*    prediction_freq = 10
* Input Layer                    [100,128]       float      dequeue
*    shape = [100,128]
* Convolution 1D                 [49,13]         float      dequeue
*    filters = 13
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 2
*    padding = valid
*    activation = linear
*    use_bias = True
*    trainable = True
*    weight = float[3,128,13]
*    bias = float[13]
* Convolution 1D                 [25,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 2
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,13,32]
* Batch Normalization            [25,32]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[32]
*    beta = float[32]
*    mean = float[32]
*    variance = float[32]
* Activation                     [25,32]         float      dequeue
*    activation = relu
*    trainable = True
* Convolution 1D                 [25,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,32,32]
* Convolution 1D                 [25,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,32,32]
* Batch Normalization            [25,32]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[32]
*    beta = float[32]
*    mean = float[32]
*    variance = float[32]
* Activation                     [25,32]         float      dequeue
*    activation = relu
*    trainable = True
* Max pooling 1D                 [12,32]         float      dequeue
*    pool_size = 2
*    strides = 2
*    padding = valid
*    trainable = True
* Dropout                        [12,32]         float      dequeue
*    rate = 0.05
*    trainable = True
* Flatten                        [384]           float      dequeue
* Dense                          [30]            float      dequeue
*    units = 30
*    use_bias = False
*    activation = linear
*    trainable = True
*    weight = float[384,30]
* Batch Normalization            [30]            float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 1
*    gamma = float[30]
*    beta = float[30]
*    mean = float[30]
*    variance = float[30]
* Activation                     [30]            float      dequeue
*    activation = relu
*    trainable = True
* Dropout                        [30]            float      dequeue
*    rate = 0.2
*    trainable = True
* Dense                          [3]             float      dequeue
*    units = 3
*    use_bias = False
*    activation = linear
*    trainable = True
*    weight = float[30,3]
* Activation                     [3]             float      dequeue
*    activation = softmax
*    trainable = True
* 
* Exported functions:
* 
* int IMAI_dequeue(float *restrict data_out)
*    Description: Dequeue features. RET_SUCCESS (0) on success, RET_NODATA (-1) if no data is available, RET_NOMEM (-2) on internal memory error
*    Parameter data_out is Output of size float[3].
* 
* int IMAI_enqueue(const float *restrict data_in)
*    Description: Enqueue features. Returns SUCCESS (0) on success, else RET_NOMEM (-2) when low on memory.
*    Parameter data_in is Input of size float[1,128,1].
* 
* void IMAI_init(void)
*    Description: Initializes buffers to initial state. This function also works as a reset function.
* 
* 
* Disclaimer:
*   The generated code relies on the optimizations done by the C compiler.
*   For example many for-loops of length 1 must be removed by the optimizer.
*   This can only be done if the functions are inlined and simplified.
*   Check disassembly if unsure.
*   tl;dr Compile using gcc with -O3 or -Ofast
*/

#ifndef _IMAI_MODEL_H_
#define _IMAI_MODEL_H_
#ifdef _MSC_VER
#pragma once
#endif

#include <stdint.h>

typedef struct {    
    char *name;
    double TP; // True Positive or Correct Positive Prediction
    double FN; // False Negative or Incorrect Negative Prediction
    double FP; // False Positive or Incorrect Positive Prediction
    double TN; // True Negative or Correct Negative Prediction
    double TPR; // True Positive Rate or Sensitivity, Recall
    double TNR; // True Negative Rate or Specificity, Selectivity
    double PPV; // Positive Predictive Value or Precision
    double NPV; // Negative Predictive Value
    double FNR; // False Negative Rate or Miss Rate
    double FPR; // False Positive Rate or Fall-Out
    double FDR; // False Discovery Rate
    double FOR; // False Omission Rate
    double F1S; // F1 Score
} IMAI_stats;

/*
* Tensorflow Test Set
* 
* (ACC) Accuracy 83.170 %
* (F1S) F1 Score 82.515 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1816             1960             1052
* (FN) False Negative or Incorrect Negative Prediction              276               14              687
* (FP) False Positive or Incorrect Positive Prediction              254              634               89
* (TN) True Negative or Correct Negative Prediction                3459             3197             3977
* (TPR) True Positive Rate or Sensitivity, Recall               86.81 %          99.29 %          60.49 %
* (TNR) True Negative Rate or Specificity, Selectivity          93.16 %          83.45 %          97.81 %
* (PPV) Positive Predictive Value or Precision                  87.73 %          75.56 %          92.20 %
* (NPV) Negative Predictive Value                               92.61 %          99.56 %          85.27 %
* (FNR) False Negative Rate or Miss Rate                        13.19 %           0.71 %          39.51 %
* (FPR) False Positive Rate or Fall-Out                          6.84 %          16.55 %           2.19 %
* (FDR) False Discovery Rate                                    12.27 %          24.44 %           7.80 %
* (FOR) False Omission Rate                                      7.39 %           0.44 %          14.73 %
* (F1S) F1 Score                                                87.27 %          85.81 %          73.06 %
*/


#define IMAI_TEST_AVG_ACC 0.8316968130921619 // Accuracy
#define IMAI_TEST_AVG_F1S 0.8251525967030483 // F1 Score

#define IMAI_TEST_STATS { \
 {name: "unlabeled", TP: 1816, FN: 276, FP: 254, TN: 3459, TPR: 0.8680688336520, TNR: 0.9315917048208, PPV: 0.8772946859903, NPV: 0.9261044176706, FNR: 0.1319311663479, FPR: 0.0684082951791, FDR: 0.1227053140096, FOR: 0.0738955823293, F1S: 0.8726573762614, }, \
 {name: "empty_box", TP: 1960, FN: 14, FP: 634, TN: 3197, TPR: 0.9929078014184, TNR: 0.8345079613677, PPV: 0.7555898226676, NPV: 0.9956399875428, FNR: 0.0070921985815, FPR: 0.1654920386322, FDR: 0.2444101773323, FOR: 0.0043600124571, F1S: 0.8581436077057, }, \
 {name: "table", TP: 1052, FN: 687, FP: 89, TN: 3977, TPR: 0.6049453709028, TNR: 0.9781111657648, PPV: 0.9219982471516, NPV: 0.8527015437392, FNR: 0.3950546290971, FPR: 0.0218888342351, FDR: 0.0780017528483, FOR: 0.1472984562607, F1S: 0.7305555555555, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_test_stats[] = IMAI_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 87.093 %
* (F1S) F1 Score 86.954 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                5342             5425             3599
* (FN) False Negative or Incorrect Negative Prediction              527                8             1594
* (FP) False Positive or Incorrect Positive Prediction              211             1810              108
* (TN) True Negative or Correct Negative Prediction               10415             9252            11194
* (TPR) True Positive Rate or Sensitivity, Recall               91.02 %          99.85 %          69.30 %
* (TNR) True Negative Rate or Specificity, Selectivity          98.01 %          83.64 %          99.04 %
* (PPV) Positive Predictive Value or Precision                  96.20 %          74.98 %          97.09 %
* (NPV) Negative Predictive Value                               95.18 %          99.91 %          87.54 %
* (FNR) False Negative Rate or Miss Rate                         8.98 %           0.15 %          30.70 %
* (FPR) False Positive Rate or Fall-Out                          1.99 %          16.36 %           0.96 %
* (FDR) False Discovery Rate                                     3.80 %          25.02 %           2.91 %
* (FOR) False Omission Rate                                      4.82 %           0.09 %          12.46 %
* (F1S) F1 Score                                                93.54 %          85.65 %          80.88 %
*/


#define IMAI_TRAIN_AVG_ACC 0.8709305850257654 // Accuracy
#define IMAI_TRAIN_AVG_F1S 0.8695366210973229 // F1 Score

#define IMAI_TRAIN_STATS { \
 {name: "unlabeled", TP: 5342, FN: 527, FP: 211, TN: 10415, TPR: 0.9102061680013, TNR: 0.9801430453604, PPV: 0.9620025211597, NPV: 0.9518369585084, FNR: 0.0897938319986, FPR: 0.0198569546395, FDR: 0.0379974788402, FOR: 0.0481630414915, F1S: 0.9353878480126, }, \
 {name: "empty_box", TP: 5425, FN: 8, FP: 1810, TN: 9252, TPR: 0.9985275170255, TNR: 0.8363767853914, PPV: 0.7498272287491, NPV: 0.9991360691144, FNR: 0.0014724829744, FPR: 0.1636232146085, FDR: 0.2501727712508, FOR: 0.0008639308855, F1S: 0.8564887906536, }, \
 {name: "table", TP: 3599, FN: 1594, FP: 108, TN: 11194, TPR: 0.6930483342961, TNR: 0.9904441691735, PPV: 0.9708659293229, NPV: 0.8753518923991, FNR: 0.3069516657038, FPR: 0.0095558308264, FDR: 0.0291340706770, FOR: 0.1246481076008, F1S: 0.8087640449438, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_train_stats[] = IMAI_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 94.873 %
* (F1S) F1 Score 94.824 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1853             1869             1885
* (FN) False Negative or Incorrect Negative Prediction              240               32               31
* (FP) False Positive or Incorrect Positive Prediction               51              152              100
* (TN) True Negative or Correct Negative Prediction                3766             3857             3894
* (TPR) True Positive Rate or Sensitivity, Recall               88.53 %          98.32 %          98.38 %
* (TNR) True Negative Rate or Specificity, Selectivity          98.66 %          96.21 %          97.50 %
* (PPV) Positive Predictive Value or Precision                  97.32 %          92.48 %          94.96 %
* (NPV) Negative Predictive Value                               94.01 %          99.18 %          99.21 %
* (FNR) False Negative Rate or Miss Rate                        11.47 %           1.68 %           1.62 %
* (FPR) False Positive Rate or Fall-Out                          1.34 %           3.79 %           2.50 %
* (FDR) False Discovery Rate                                     2.68 %           7.52 %           5.04 %
* (FOR) False Omission Rate                                      5.99 %           0.82 %           0.79 %
* (F1S) F1 Score                                                92.72 %          95.31 %          96.64 %
*/


#define IMAI_VALIDATION_AVG_ACC 0.9487309644670051 // Accuracy
#define IMAI_VALIDATION_AVG_F1S 0.9482391547251722 // F1 Score

#define IMAI_VALIDATION_STATS { \
 {name: "unlabeled", TP: 1853, FN: 240, FP: 51, TN: 3766, TPR: 0.8853320592451, TNR: 0.9866387215090, PPV: 0.9732142857142, NPV: 0.9400898652021, FNR: 0.1146679407548, FPR: 0.0133612784909, FDR: 0.0267857142857, FOR: 0.0599101347978, F1S: 0.9271953965474, }, \
 {name: "empty_box", TP: 1869, FN: 32, FP: 152, TN: 3857, TPR: 0.9831667543398, TNR: 0.9620853080568, PPV: 0.9247897080653, NPV: 0.9917716636667, FNR: 0.0168332456601, FPR: 0.0379146919431, FDR: 0.0752102919346, FOR: 0.0082283363332, F1S: 0.9530851606323, }, \
 {name: "table", TP: 1885, FN: 31, FP: 100, TN: 3894, TPR: 0.9838204592901, TNR: 0.9749624436654, PPV: 0.9496221662468, NPV: 0.9921019108280, FNR: 0.0161795407098, FPR: 0.0250375563345, FDR: 0.0503778337531, FOR: 0.0078980891719, F1S: 0.9664188669571, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_validation_stats[] = IMAI_VALIDATION_STATS;
#endif

#define IMAI_API_QUEUE

// All symbols in order
#define IMAI_SYMBOL_MAP {"(unlabeled)", "empty_box", "table"}

// Model GUID (16 bytes)
#define IMAI_MODEL_ID {0xac, 0x8f, 0x5c, 0x71, 0x23, 0x1c, 0xce, 0x45, 0x9d, 0xbf, 0x21, 0xf5, 0x87, 0x5d, 0x9c, 0xe5}

// First nibble is bit encoding, second nibble is number of bytes
#define IMAGINET_TYPES_NONE	(0x0)
#define IMAGINET_TYPES_FLOAT32	(0x14)
#define IMAGINET_TYPES_FLOAT64	(0x18)
#define IMAGINET_TYPES_INT8	(0x21)
#define IMAGINET_TYPES_INT16	(0x22)
#define IMAGINET_TYPES_INT32	(0x24)
#define IMAGINET_TYPES_INT64	(0x28)
#define IMAGINET_TYPES_QDYN8	(0x31)
#define IMAGINET_TYPES_QDYN16	(0x32)
#define IMAGINET_TYPES_QDYN32	(0x34)

// data_in [1,128,1] (512 bytes)
#define IMAI_DATA_IN_COUNT (128)
#define IMAI_DATA_IN_TYPE float
#define IMAI_DATA_IN_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_DATA_IN_SCALE (1)
#define IMAI_DATA_IN_OFFSET (0)
#define IMAI_DATA_IN_IS_QUANTIZED (0)

// data_out [3] (12 bytes)
#define IMAI_DATA_OUT_COUNT (3)
#define IMAI_DATA_OUT_TYPE float
#define IMAI_DATA_OUT_TYPE_ID IMAGINET_TYPES_FLOAT32
#define IMAI_DATA_OUT_SCALE (1)
#define IMAI_DATA_OUT_OFFSET (0)
#define IMAI_DATA_OUT_IS_QUANTIZED (0)

#define IMAI_KEY_MAX (37)



// Return codes
#define IMAI_RET_SUCCESS 0
#define IMAI_RET_NODATA -1
#define IMAI_RET_NOMEM -2

// Exported methods
int IMAI_dequeue(float *restrict data_out);
int IMAI_enqueue(const float *restrict data_in);
void IMAI_init(void);

#endif /* _IMAI_MODEL_H_ */
