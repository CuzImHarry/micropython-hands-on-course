/*
* DEEPCRAFT Studio 5.9.4563.0+34bdb7f4372a1120ca38a0cb02e62db5b4b78270
* Copyright © 2023- Imagimob AB, All Rights Reserved.
* 
* Generated at 02/21/2026 09:46:55 UTC. Any changes will be lost.
* 
* Model ID  77641dd1-5aed-4d64-b02f-436b930dbe7c
* 
* Memory    Size                      Efficiency
* Buffers   29200 bytes (RAM)         100 %
* State     26208 bytes (RAM)         100 %
* Readonly  79648 bytes (Flash)       100 %
* 
* Backend              tensorflow
* Keras Version        2.15.0
* Backend Model Type   Sequential
* Backend Model Name   conv1d-small-balanced-1
* 
* Class Index | Symbol Label
* 0           | (unlabeled)
* 1           | empty_box
* 2           | table
* 
* Layer                          Shape           Type       Function
* Sliding Window (data points)   [100,65]        float      dequeue
*    window_shape = [100,65]
*    stride = 1300
*    buffer_multiplier = 1
* Contextual Window (Sliding Window) [100,65]        float      dequeue
*    contextual_length_sec = 0.5
*    prediction_freq = 10
* Input Layer                    [100,65]        float      dequeue
*    shape = [100,65]
* Convolution 1D                 [50,16]         float      dequeue
*    filters = 16
*    kernel_size = 5
*    dilation_rate = 1
*    strides = 2
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[5,65,16]
* Batch Normalization            [50,16]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[16]
*    beta = float[16]
*    mean = float[16]
*    variance = float[16]
* Activation                     [50,16]         float      dequeue
*    activation = relu
*    trainable = True
* Convolution 1D                 [50,16]         float      dequeue
*    filters = 16
*    kernel_size = 5
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[5,16,16]
* Convolution 1D                 [50,16]         float      dequeue
*    filters = 16
*    kernel_size = 5
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[5,16,16]
* Batch Normalization            [50,16]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[16]
*    beta = float[16]
*    mean = float[16]
*    variance = float[16]
* Activation                     [50,16]         float      dequeue
*    activation = relu
*    trainable = True
* Max pooling 1D                 [25,16]         float      dequeue
*    pool_size = 2
*    strides = 2
*    padding = valid
*    trainable = True
* Flatten                        [400]           float      dequeue
* Dense                          [30]            float      dequeue
*    units = 30
*    use_bias = False
*    activation = linear
*    trainable = True
*    weight = float[400,30]
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
*    rate = 0.1
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
*    Parameter data_in is Input of size float[1,65,1].
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
* (ACC) Accuracy 72.145 %
* (F1S) F1 Score 69.708 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1683             3026              510
* (FN) False Negative or Incorrect Negative Prediction              728                4             1283
* (FP) False Positive or Incorrect Positive Prediction                4             1894              117
* (TN) True Negative or Correct Negative Prediction                4819             2310             5324
* (TPR) True Positive Rate or Sensitivity, Recall               69.81 %          99.87 %          28.44 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.92 %          54.95 %          97.85 %
* (PPV) Positive Predictive Value or Precision                  99.76 %          61.50 %          81.34 %
* (NPV) Negative Predictive Value                               86.88 %          99.83 %          80.58 %
* (FNR) False Negative Rate or Miss Rate                        30.19 %           0.13 %          71.56 %
* (FPR) False Positive Rate or Fall-Out                          0.08 %          45.05 %           2.15 %
* (FDR) False Discovery Rate                                     0.24 %          38.50 %          18.66 %
* (FOR) False Omission Rate                                     13.12 %           0.17 %          19.42 %
* (F1S) F1 Score                                                82.14 %          76.13 %          42.15 %
*/


#define IMAI_TEST_AVG_ACC 0.7214542438484932 // Accuracy
#define IMAI_TEST_AVG_F1S 0.6970800120603817 // F1 Score

#define IMAI_TEST_STATS { \
 {name: "unlabeled", TP: 1683, FN: 728, FP: 4, TN: 4819, TPR: 0.6980506014102, TNR: 0.9991706406800, PPV: 0.9976289270895, NPV: 0.8687578871462, FNR: 0.3019493985897, FPR: 0.0008293593199, FDR: 0.0023710729104, FOR: 0.1312421128537, F1S: 0.8213762811127, }, \
 {name: "empty_box", TP: 3026, FN: 4, FP: 1894, TN: 2310, TPR: 0.9986798679867, TNR: 0.5494766888677, PPV: 0.6150406504065, NPV: 0.9982713915298, FNR: 0.0013201320132, FPR: 0.4505233111322, FDR: 0.3849593495934, FOR: 0.0017286084701, F1S: 0.7612578616352, }, \
 {name: "table", TP: 510, FN: 1283, FP: 117, TN: 5324, TPR: 0.2844394868934, TNR: 0.9784965998897, PPV: 0.8133971291866, NPV: 0.8058120175571, FNR: 0.7155605131065, FPR: 0.0215034001102, FDR: 0.1866028708133, FOR: 0.1941879824428, F1S: 0.4214876033057, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_test_stats[] = IMAI_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 86.071 %
* (F1S) F1 Score 85.712 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                3736            11268             7538
* (FN) False Negative or Incorrect Negative Prediction             1985               23             1640
* (FP) False Positive or Incorrect Positive Prediction               77             3059              512
* (TN) True Negative or Correct Negative Prediction               20392            11840            16500
* (TPR) True Positive Rate or Sensitivity, Recall               65.30 %          99.80 %          82.13 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.62 %          79.47 %          96.99 %
* (PPV) Positive Predictive Value or Precision                  97.98 %          78.65 %          93.64 %
* (NPV) Negative Predictive Value                               91.13 %          99.81 %          90.96 %
* (FNR) False Negative Rate or Miss Rate                        34.70 %           0.20 %          17.87 %
* (FPR) False Positive Rate or Fall-Out                          0.38 %          20.53 %           3.01 %
* (FDR) False Discovery Rate                                     2.02 %          21.35 %           6.36 %
* (FOR) False Omission Rate                                      8.87 %           0.19 %           9.04 %
* (F1S) F1 Score                                                78.37 %          87.97 %          87.51 %
*/


#define IMAI_TRAIN_AVG_ACC 0.8607101947308133 // Accuracy
#define IMAI_TRAIN_AVG_F1S 0.857115078267004 // F1 Score

#define IMAI_TRAIN_STATS { \
 {name: "unlabeled", TP: 3736, FN: 1985, FP: 77, TN: 20392, TPR: 0.6530326865932, TNR: 0.9962382138844, PPV: 0.9798059270915, NPV: 0.9112928453322, FNR: 0.3469673134067, FPR: 0.0037617861155, FDR: 0.0201940729084, FOR: 0.0887071546677, F1S: 0.7837214180826, }, \
 {name: "empty_box", TP: 11268, FN: 23, FP: 3059, TN: 11840, TPR: 0.9979629793640, TNR: 0.7946842069937, PPV: 0.7864870524185, NPV: 0.9980611986849, FNR: 0.0020370206359, FPR: 0.2053157930062, FDR: 0.2135129475814, FOR: 0.0019388013150, F1S: 0.8796939651807, }, \
 {name: "table", TP: 7538, FN: 1640, FP: 512, TN: 16500, TPR: 0.8213118326432, TNR: 0.9699035974606, PPV: 0.9363975155279, NPV: 0.9095920617420, FNR: 0.1786881673567, FPR: 0.0300964025393, FDR: 0.0636024844720, FOR: 0.0904079382579, F1S: 0.8750870675644, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_train_stats[] = IMAI_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 85.966 %
* (F1S) F1 Score 84.273 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                 435             4284             2264
* (FN) False Negative or Incorrect Negative Prediction              757               95              288
* (FP) False Positive or Incorrect Positive Prediction               16              852              272
* (TN) True Negative or Correct Negative Prediction                6915             2892             5299
* (TPR) True Positive Rate or Sensitivity, Recall               36.49 %          97.83 %          88.71 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.77 %          77.24 %          95.12 %
* (PPV) Positive Predictive Value or Precision                  96.45 %          83.41 %          89.27 %
* (NPV) Negative Predictive Value                               90.13 %          96.82 %          94.85 %
* (FNR) False Negative Rate or Miss Rate                        63.51 %           2.17 %          11.29 %
* (FPR) False Positive Rate or Fall-Out                          0.23 %          22.76 %           4.88 %
* (FDR) False Discovery Rate                                     3.55 %          16.59 %          10.73 %
* (FOR) False Omission Rate                                      9.87 %           3.18 %           5.15 %
* (F1S) F1 Score                                                52.95 %          90.05 %          88.99 %
*/


#define IMAI_VALIDATION_AVG_ACC 0.8596577619106242 // Accuracy
#define IMAI_VALIDATION_AVG_F1S 0.8427277290829903 // F1 Score

#define IMAI_VALIDATION_STATS { \
 {name: "unlabeled", TP: 435, FN: 757, FP: 16, TN: 6915, TPR: 0.3649328859060, TNR: 0.9976915308036, PPV: 0.9645232815964, NPV: 0.9013295099061, FNR: 0.6350671140939, FPR: 0.0023084691963, FDR: 0.0354767184035, FOR: 0.0986704900938, F1S: 0.5295191722458, }, \
 {name: "empty_box", TP: 4284, FN: 95, FP: 852, TN: 2892, TPR: 0.9783055492121, TNR: 0.7724358974358, PPV: 0.8341121495327, NPV: 0.9681955138935, FNR: 0.0216944507878, FPR: 0.2275641025641, FDR: 0.1658878504672, FOR: 0.0318044861064, F1S: 0.9004729374671, }, \
 {name: "table", TP: 2264, FN: 288, FP: 272, TN: 5299, TPR: 0.8871473354231, TNR: 0.9511757314665, PPV: 0.8927444794952, NPV: 0.9484517630212, FNR: 0.1128526645768, FPR: 0.0488242685334, FDR: 0.1072555205047, FOR: 0.0515482369787, F1S: 0.8899371069182, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_validation_stats[] = IMAI_VALIDATION_STATS;
#endif

#define IMAI_API_QUEUE

// All symbols in order
#define IMAI_SYMBOL_MAP {"(unlabeled)", "empty_box", "table"}

// Model GUID (16 bytes)
#define IMAI_MODEL_ID {0xd1, 0x1d, 0x64, 0x77, 0xed, 0x5a, 0x64, 0x4d, 0xb0, 0x2f, 0x43, 0x6b, 0x93, 0x0d, 0xbe, 0x7c}

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

// data_in [1,65,1] (260 bytes)
#define IMAI_DATA_IN_COUNT (65)
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

#define IMAI_KEY_MAX (33)



// Return codes
#define IMAI_RET_SUCCESS 0
#define IMAI_RET_NODATA -1
#define IMAI_RET_NOMEM -2

// Exported methods
int IMAI_dequeue(float *restrict data_out);
int IMAI_enqueue(const float *restrict data_in);
void IMAI_init(void);

#endif /* _IMAI_MODEL_H_ */
