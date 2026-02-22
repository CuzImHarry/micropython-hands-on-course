/*
* DEEPCRAFT Studio 5.9.4563.0+34bdb7f4372a1120ca38a0cb02e62db5b4b78270
* Copyright © 2023- Imagimob AB, All Rights Reserved.
* 
* Generated at 02/20/2026 09:39:12 UTC. Any changes will be lost.
* 
* Model ID  0f568d7d-04cf-4ec1-b3a8-a1010b767977
* 
* Memory    Size                      Efficiency
* Buffers   29200 bytes (RAM)         100 %
* State     26208 bytes (RAM)         100 %
* Readonly  73248 bytes (Flash)       100 %
* 
* Backend              tensorflow
* Keras Version        2.15.0
* Backend Model Type   Sequential
* Backend Model Name   conv1d-medium-balanced-1
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
* Convolution 1D                 [25,16]         float      dequeue
*    filters = 16
*    kernel_size = 5
*    dilation_rate = 1
*    strides = 2
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[5,16,16]
* Convolution 1D                 [13,16]         float      dequeue
*    filters = 16
*    kernel_size = 5
*    dilation_rate = 1
*    strides = 2
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[5,16,16]
* Batch Normalization            [13,16]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[16]
*    beta = float[16]
*    mean = float[16]
*    variance = float[16]
* Activation                     [13,16]         float      dequeue
*    activation = relu
*    trainable = True
* Convolution 1D                 [13,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,16,32]
* Convolution 1D                 [13,32]         float      dequeue
*    filters = 32
*    kernel_size = 3
*    dilation_rate = 1
*    strides = 1
*    padding = same
*    activation = linear
*    use_bias = False
*    trainable = True
*    weight = float[3,32,32]
* Batch Normalization            [13,32]         float      dequeue
*    epsilon = 0.001
*    trainable = True
*    scale = True
*    center = True
*    axis = 2
*    gamma = float[32]
*    beta = float[32]
*    mean = float[32]
*    variance = float[32]
* Activation                     [13,32]         float      dequeue
*    activation = relu
*    trainable = True
* Max pooling 1D                 [6,32]          float      dequeue
*    pool_size = 2
*    strides = 2
*    padding = valid
*    trainable = True
* Flatten                        [192]           float      dequeue
* Dense                          [30]            float      dequeue
*    units = 30
*    use_bias = False
*    activation = linear
*    trainable = True
*    weight = float[192,30]
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
* (ACC) Accuracy 76.867 %
* (F1S) F1 Score 76.475 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                1615             2067              930
* (FN) False Negative or Incorrect Negative Prediction              572               13              803
* (FP) False Positive or Incorrect Positive Prediction               13             1115              260
* (TN) True Negative or Correct Negative Prediction                3800             2805             4007
* (TPR) True Positive Rate or Sensitivity, Recall               73.85 %          99.38 %          53.66 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.66 %          71.56 %          93.91 %
* (PPV) Positive Predictive Value or Precision                  99.20 %          64.96 %          78.15 %
* (NPV) Negative Predictive Value                               86.92 %          99.54 %          83.31 %
* (FNR) False Negative Rate or Miss Rate                        26.15 %           0.63 %          46.34 %
* (FPR) False Positive Rate or Fall-Out                          0.34 %          28.44 %           6.09 %
* (FDR) False Discovery Rate                                     0.80 %          35.04 %          21.85 %
* (FOR) False Omission Rate                                     13.08 %           0.46 %          16.69 %
* (F1S) F1 Score                                                84.67 %          78.56 %          63.63 %
*/


#define IMAI_TEST_AVG_ACC 0.7686666666666667 // Accuracy
#define IMAI_TEST_AVG_F1S 0.7647535800127857 // F1 Score

#define IMAI_TEST_STATS { \
 {name: "unlabeled", TP: 1615, FN: 572, FP: 13, TN: 3800, TPR: 0.7384545038866, TNR: 0.9965906110674, PPV: 0.9920147420147, NPV: 0.8691674290942, FNR: 0.2615454961133, FPR: 0.0034093889325, FDR: 0.0079852579852, FOR: 0.1308325709057, F1S: 0.8466579292267, }, \
 {name: "empty_box", TP: 2067, FN: 13, FP: 1115, TN: 2805, TPR: 0.99375, TNR: 0.7155612244897, PPV: 0.6495914519170, NPV: 0.9953867991483, FNR: 0.00625, FPR: 0.2844387755102, FDR: 0.3504085480829, FOR: 0.0046132008516, F1S: 0.7856328392246, }, \
 {name: "table", TP: 930, FN: 803, FP: 260, TN: 4007, TPR: 0.5366416618580, TNR: 0.9390672603702, PPV: 0.7815126050420, NPV: 0.8330561330561, FNR: 0.4633583381419, FPR: 0.0609327396297, FDR: 0.2184873949579, FOR: 0.1669438669438, F1S: 0.6363325350667, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_test_stats[] = IMAI_TEST_STATS;
#endif

/*
* Tensorflow Train Set
* 
* (ACC) Accuracy 92.904 %
* (F1S) F1 Score 92.827 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                3880             6642             7206
* (FN) False Negative or Incorrect Negative Prediction              777              539               38
* (FP) False Positive or Incorrect Positive Prediction              124              354              876
* (TN) True Negative or Correct Negative Prediction               14301            11547            10962
* (TPR) True Positive Rate or Sensitivity, Recall               83.32 %          92.49 %          99.48 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.14 %          97.03 %          92.60 %
* (PPV) Positive Predictive Value or Precision                  96.90 %          94.94 %          89.16 %
* (NPV) Negative Predictive Value                               94.85 %          95.54 %          99.65 %
* (FNR) False Negative Rate or Miss Rate                        16.68 %           7.51 %           0.52 %
* (FPR) False Positive Rate or Fall-Out                          0.86 %           2.97 %           7.40 %
* (FDR) False Discovery Rate                                     3.10 %           5.06 %          10.84 %
* (FOR) False Omission Rate                                      5.15 %           4.46 %           0.35 %
* (F1S) F1 Score                                                89.60 %          93.70 %          94.04 %
*/


#define IMAI_TRAIN_AVG_ACC 0.9290430772455718 // Accuracy
#define IMAI_TRAIN_AVG_F1S 0.9282672586135077 // F1 Score

#define IMAI_TRAIN_STATS { \
 {name: "unlabeled", TP: 3880, FN: 777, FP: 124, TN: 14301, TPR: 0.8331543912389, TNR: 0.9914038128249, PPV: 0.9690309690309, NPV: 0.9484679665738, FNR: 0.1668456087610, FPR: 0.0085961871750, FDR: 0.0309690309690, FOR: 0.0515320334261, F1S: 0.8959704422122, }, \
 {name: "empty_box", TP: 6642, FN: 539, FP: 354, TN: 11547, TPR: 0.9249408160423, TNR: 0.9702546004537, PPV: 0.9493996569468, NPV: 0.9554029455568, FNR: 0.0750591839576, FPR: 0.0297453995462, FDR: 0.0506003430531, FOR: 0.0445970544431, F1S: 0.9370106510545, }, \
 {name: "table", TP: 7206, FN: 38, FP: 876, TN: 10962, TPR: 0.9947542794036, TNR: 0.9260010136847, PPV: 0.8916109873793, NPV: 0.9965454545454, FNR: 0.0052457205963, FPR: 0.0739989863152, FDR: 0.1083890126206, FOR: 0.0034545454545, F1S: 0.9403627822001, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_train_stats[] = IMAI_TRAIN_STATS;
#endif

/*
* Tensorflow Validation Set
* 
* (ACC) Accuracy 91.267 %
* (F1S) F1 Score 89.876 %
* 
* Name of class                                               unlabeled        empty_box            table
* (TP) True Positive or Correct Positive Prediction                 419             3358             1699
* (FN) False Negative or Incorrect Negative Prediction              511               13                0
* (FP) False Positive or Incorrect Positive Prediction               12              314              198
* (TN) True Negative or Correct Negative Prediction                5058             2315             4103
* (TPR) True Positive Rate or Sensitivity, Recall               45.05 %          99.61 %         100.00 %
* (TNR) True Negative Rate or Specificity, Selectivity          99.76 %          88.06 %          95.40 %
* (PPV) Positive Predictive Value or Precision                  97.22 %          91.45 %          89.56 %
* (NPV) Negative Predictive Value                               90.82 %          99.44 %         100.00 %
* (FNR) False Negative Rate or Miss Rate                        54.95 %           0.39 %           0.00 %
* (FPR) False Positive Rate or Fall-Out                          0.24 %          11.94 %           4.60 %
* (FDR) False Discovery Rate                                     2.78 %           8.55 %          10.44 %
* (FOR) False Omission Rate                                      9.18 %           0.56 %           0.00 %
* (F1S) F1 Score                                                61.57 %          95.36 %          94.49 %
*/


#define IMAI_VALIDATION_AVG_ACC 0.9126666666666666 // Accuracy
#define IMAI_VALIDATION_AVG_F1S 0.8987602840529557 // F1 Score

#define IMAI_VALIDATION_STATS { \
 {name: "unlabeled", TP: 419, FN: 511, FP: 12, TN: 5058, TPR: 0.4505376344086, TNR: 0.9976331360946, PPV: 0.9721577726218, NPV: 0.9082420542287, FNR: 0.5494623655913, FPR: 0.0023668639053, FDR: 0.0278422273781, FOR: 0.0917579457712, F1S: 0.6157237325495, }, \
 {name: "empty_box", TP: 3358, FN: 13, FP: 314, TN: 2315, TPR: 0.9961435775734, TNR: 0.8805629516926, PPV: 0.9144880174291, NPV: 0.9944158075601, FNR: 0.0038564224265, FPR: 0.1194370483073, FDR: 0.0855119825708, FOR: 0.0055841924398, F1S: 0.9535709214823, }, \
 {name: "table", TP: 1699, FN: 0, FP: 198, TN: 4103, TPR: 1, TNR: 0.9539641943734, PPV: 0.8956246705324, NPV: 1, FNR: 0, FPR: 0.0460358056265, FDR: 0.1043753294675, FOR: 0, F1S: 0.9449388209121, }, \
}

#ifdef IMAI_STATS_ENABLED
static const IMAI_stats IMAI_validation_stats[] = IMAI_VALIDATION_STATS;
#endif

#define IMAI_API_QUEUE

// All symbols in order
#define IMAI_SYMBOL_MAP {"(unlabeled)", "empty_box", "table"}

// Model GUID (16 bytes)
#define IMAI_MODEL_ID {0x7d, 0x8d, 0x56, 0x0f, 0xcf, 0x04, 0xc1, 0x4e, 0xb3, 0xa8, 0xa1, 0x01, 0x0b, 0x76, 0x79, 0x77}

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

#define IMAI_KEY_MAX (42)



// Return codes
#define IMAI_RET_SUCCESS 0
#define IMAI_RET_NODATA -1
#define IMAI_RET_NOMEM -2

// Exported methods
int IMAI_dequeue(float *restrict data_out);
int IMAI_enqueue(const float *restrict data_in);
void IMAI_init(void);

#endif /* _IMAI_MODEL_H_ */
