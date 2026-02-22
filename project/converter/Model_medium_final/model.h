/*
* DEEPCRAFT Studio 5.9.4563.0+34bdb7f4372a1120ca38a0cb02e62db5b4b78270
* Copyright © 2023- Imagimob AB, All Rights Reserved.
* 
* Generated at 02/22/2026 19:01:05 UTC. Any changes will be lost.
* 
* Model ID  4cd4412e-f742-4754-a0e5-cf3a96976bb5
* 
* Memory    Size                      Efficiency
* Buffers   54400 bytes (RAM)         100 %
* State     51408 bytes (RAM)         100 %
* Readonly  209824 bytes (Flash)      100 %
* 
* Backend              tensorflow
* Keras Version        2.15.0
* Backend Model Type   Sequential
* Backend Model Name   conv1d-medium-balanced-3_1
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
* Convolution 1D                 [50,16]         float      dequeue
*    filters = 16
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 2
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,128,16]
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
* Convolution 1D                 [50,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,16,32]
* Convolution 1D                 [50,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,32,32]
* Batch Normalization            [50,32]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[32]
*    beta = float[32]
*    mean = float[32]
*    variance = float[32]
* Activation                     [50,32]         float      dequeue
*    activation = relu
*    trainable = True
* Max pooling 1D                 [25,32]         float      dequeue
*    pool_size = 2
*    strides = 2
*    padding = valid
*    trainable = True
* Convolution 1D                 [25,64]         float      dequeue
*    filters = 64
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,32,64]
* Convolution 1D                 [25,64]         float      dequeue
*    filters = 64
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,64,64]
* Batch Normalization            [25,64]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[64]
*    beta = float[64]
*    mean = float[64]
*    variance = float[64]
* Activation                     [25,64]         float      dequeue
*    activation = relu
*    trainable = True
* Max pooling 1D                 [12,64]         float      dequeue
*    pool_size = 2
*    strides = 2
*    padding = valid
*    trainable = True
* Flatten                        [768]           float      dequeue
* Dense                          [30]            float      dequeue
*    units = 30
*    use_bias = False
*    activation = linear
*    trainable = True
*    weight = float[768,30]
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
* (ACC) Accuracy 95.211 %
* (F1S) F1 Score 95.162 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1861             1937             1729
* (FN) False Negative or Incorrect Negative Prediction              231               37               10
* (FP) False Positive or Incorrect Positive Prediction               46               95              137
* (TN) True Negative or Correct Negative Prediction                3667             3736             3929
* (TPR) True Positive Rate or Sensitivity, Recall               88.96 %          98.13 %          99.42 %
* (TNR) True Negative Rate or Specificity, Selectivity          98.76 %          97.52 %          96.63 %
* (PPV) Positive Predictive Value or Precision                  97.59 %          95.32 %          92.66 %
* (NPV) Negative Predictive Value                               94.07 %          99.02 %          99.75 %
* (FNR) False Negative Rate or Miss Rate                        11.04 %           1.87 %           0.58 %
* (FPR) False Positive Rate or Fall-Out                          1.24 %           2.48 %           3.37 %
* (FDR) False Discovery Rate                                     2.41 %           4.68 %           7.34 %
* (FOR) False Omission Rate                                      5.93 %           0.98 %           0.25 %
* (F1S) F1 Score                                                93.07 %          96.70 %          95.92 %
*/


#define IMAI_TEST_AVG_ACC 0.9521102497846684 // Accuracy
#define IMAI_TEST_AVG_F1S 0.9516171679985655 // F1 Score

#define IMAI_TEST_STATS { \
 {name: "unlabeled", TP: 1861, FN: 231, FP: 46, TN: 3667, TPR: 0.8895793499043, TNR: 0.9876110961486, PPV: 0.9758783429470, NPV: 0.9407388404309, FNR: 0.1104206500956, FPR: 0.0123889038513, FDR: 0.0241216570529, FOR: 0.0592611595690, F1S: 0.9307326831707, }, \
 {name: "empty_box", TP: 1937, FN: 37, FP: 95, TN: 3736, TPR: 0.9812563323201, TNR: 0.9752022970503, PPV: 0.9532480314960, NPV: 0.9901934799893, FNR: 0.0187436676798, FPR: 0.0247977029496, FDR: 0.0467519685039, FOR: 0.0098065200106, F1S: 0.9670494258612, }, \
 {name: "table", TP: 1729, FN: 10, FP: 137, TN: 3929, TPR: 0.9942495687176, TNR: 0.9663059517953, PPV: 0.9265809217577, NPV: 0.9974612845899, FNR: 0.0057504312823, FPR: 0.0336940482046, FDR: 0.0734190782422, FOR: 0.0025387154100, F1S: 0.9592233009708, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_test_stats[] = IMAI_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 95.580 %
* (F1S) F1 Score 95.535 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                5234             5413             5119
* (FN) False Negative or Incorrect Negative Prediction              635               20               74
* (FP) False Positive or Incorrect Positive Prediction               74              430              225
* (TN) True Negative or Correct Negative Prediction               10552            10632            11077
* (TPR) True Positive Rate or Sensitivity, Recall               89.18 %          99.63 %          98.58 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.30 %          96.11 %          98.01 %
* (PPV) Positive Predictive Value or Precision                  98.61 %          92.64 %          95.79 %
* (NPV) Negative Predictive Value                               94.32 %          99.81 %          99.34 %
* (FNR) False Negative Rate or Miss Rate                        10.82 %           0.37 %           1.42 %
* (FPR) False Positive Rate or Fall-Out                          0.70 %           3.89 %           1.99 %
* (FDR) False Discovery Rate                                     1.39 %           7.36 %           4.21 %
* (FOR) False Omission Rate                                      5.68 %           0.19 %           0.66 %
* (F1S) F1 Score                                                93.66 %          96.01 %          97.16 %
*/


#define IMAI_TRAIN_AVG_ACC 0.9558047893301 // Accuracy
#define IMAI_TRAIN_AVG_F1S 0.9553519432659516 // F1 Score

#define IMAI_TRAIN_STATS { \
 {name: "unlabeled", TP: 5234, FN: 635, FP: 74, TN: 10552, TPR: 0.8918043959788, TNR: 0.9930359495576, PPV: 0.9860587792012, NPV: 0.9432376866005, FNR: 0.1081956040211, FPR: 0.0069640504423, FDR: 0.0139412207987, FOR: 0.0567623133994, F1S: 0.9365661626554, }, \
 {name: "empty_box", TP: 5413, FN: 20, FP: 430, TN: 10632, TPR: 0.9963187925639, TNR: 0.9611281865847, PPV: 0.9264076672941, NPV: 0.9981224183251, FNR: 0.0036812074360, FPR: 0.0388718134152, FDR: 0.0735923327058, FOR: 0.0018775816748, F1S: 0.9600922312876, }, \
 {name: "table", TP: 5119, FN: 74, FP: 225, TN: 11077, TPR: 0.9857500481417, TNR: 0.9800920191116, PPV: 0.9578967065868, NPV: 0.9933638238722, FNR: 0.0142499518582, FPR: 0.0199079808883, FDR: 0.0421032934131, FOR: 0.0066361761277, F1S: 0.9716238018411, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_train_stats[] = IMAI_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 94.755 %
* (F1S) F1 Score 94.731 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1885             1854             1861
* (FN) False Negative or Incorrect Negative Prediction              208               47               55
* (FP) False Positive or Incorrect Positive Prediction               50              138              122
* (TN) True Negative or Correct Negative Prediction                3767             3871             3872
* (TPR) True Positive Rate or Sensitivity, Recall               90.06 %          97.53 %          97.13 %
* (TNR) True Negative Rate or Specificity, Selectivity          98.69 %          96.56 %          96.95 %
* (PPV) Positive Predictive Value or Precision                  97.42 %          93.07 %          93.85 %
* (NPV) Negative Predictive Value                               94.77 %          98.80 %          98.60 %
* (FNR) False Negative Rate or Miss Rate                         9.94 %           2.47 %           2.87 %
* (FPR) False Positive Rate or Fall-Out                          1.31 %           3.44 %           3.05 %
* (FDR) False Discovery Rate                                     2.58 %           6.93 %           6.15 %
* (FOR) False Omission Rate                                      5.23 %           1.20 %           1.40 %
* (F1S) F1 Score                                                93.59 %          95.25 %          95.46 %
*/


#define IMAI_VALIDATION_AVG_ACC 0.9475465313028765 // Accuracy
#define IMAI_VALIDATION_AVG_F1S 0.9473135210396544 // F1 Score

#define IMAI_VALIDATION_STATS { \
 {name: "unlabeled", TP: 1885, FN: 208, FP: 50, TN: 3767, TPR: 0.9006211180124, TNR: 0.9869007073618, PPV: 0.9741602067183, NPV: 0.9476729559748, FNR: 0.0993788819875, FPR: 0.0130992926381, FDR: 0.0258397932816, FOR: 0.0523270440251, F1S: 0.9359483614697, }, \
 {name: "empty_box", TP: 1854, FN: 47, FP: 138, TN: 3871, TPR: 0.9752761704366, TNR: 0.9655774507358, PPV: 0.9307228915662, NPV: 0.9880040837161, FNR: 0.0247238295633, FPR: 0.0344225492641, FDR: 0.0692771084337, FOR: 0.0119959162838, F1S: 0.9524788081171, }, \
 {name: "table", TP: 1861, FN: 55, FP: 122, TN: 3872, TPR: 0.9712943632567, TNR: 0.9694541812719, PPV: 0.9384770549672, NPV: 0.9859943977591, FNR: 0.0287056367432, FPR: 0.0305458187280, FDR: 0.0615229450327, FOR: 0.0140056022408, F1S: 0.9546037445498, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_validation_stats[] = IMAI_VALIDATION_STATS;
#endif

#define IMAI_API_QUEUE

// All symbols in order
#define IMAI_SYMBOL_MAP {"(unlabeled)", "empty_box", "table"}

// Model GUID (16 bytes)
#define IMAI_MODEL_ID {0x2e, 0x41, 0xd4, 0x4c, 0x42, 0xf7, 0x54, 0x47, 0xa0, 0xe5, 0xcf, 0x3a, 0x96, 0x97, 0x6b, 0xb5}

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

#define IMAI_KEY_MAX (44)



// Return codes
#define IMAI_RET_SUCCESS 0
#define IMAI_RET_NODATA -1
#define IMAI_RET_NOMEM -2

// Exported methods
int IMAI_dequeue(float *restrict data_out);
int IMAI_enqueue(const float *restrict data_in);
void IMAI_init(void);

#endif /* _IMAI_MODEL_H_ */
