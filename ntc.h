/*
 * VCC:+5V
 *    |
 *  <NTC>
 *    |
 *    +----> ADC
 *    |
 *   <R>
 *    |
 *   GND 
 * 
 * ADC = 1 / (NTC + R) * R * 1024
 *
 */

// NTC 50K
#define NTC50K_R47K_00C       221
#define NTC50K_R47K_01C       232
#define NTC50K_R47K_02C       243
#define NTC50K_R47K_03C       254
#define NTC50K_R47K_05C       265
#define NTC50K_R47K_06C       277
#define NTC50K_R47K_07C       288
#define NTC50K_R47K_08C       300
#define NTC50K_R47K_09C       313
#define NTC50K_R47K_10C       325
#define NTC50K_R47K_11C       338
#define NTC50K_R47K_12C       351
#define NTC50K_R47K_13C       363
#define NTC50K_R47K_15C       376
#define NTC50K_R47K_16C       390
#define NTC50K_R47K_17C       403
#define NTC50K_R47K_18C       416
#define NTC50K_R47K_19C       429
#define NTC50K_R47K_20C       443
#define NTC50K_R47K_21C       456
#define NTC50K_R47K_22C       470
#define NTC50K_R47K_23C       483
#define NTC50K_R47K_25C       496
#define NTC50K_R47K_26C       509
#define NTC50K_R47K_27C       523
#define NTC50K_R47K_28C       536
#define NTC50K_R47K_29C       549
#define NTC50K_R47K_30C       561
#define NTC50K_R47K_31C       574
#define NTC50K_R47K_32C       587
#define NTC50K_R47K_33C       599
#define NTC50K_R47K_35C       611
#define NTC50K_R47K_36C       623
#define NTC50K_R47K_37C       635
#define NTC50K_R47K_38C       647
#define NTC50K_R47K_39C       658
#define NTC50K_R47K_40C       669
#define NTC50K_R47K_41C       680
#define NTC50K_R47K_42C       691
#define NTC50K_R47K_43C       701
#define NTC50K_R47K_45C       711
#define NTC50K_R47K_46C       721
#define NTC50K_R47K_47C       731
#define NTC50K_R47K_48C       741
#define NTC50K_R47K_49C       750
#define NTC50K_R47K_50C       759
#define NTC50K_R47K_51C       768
#define NTC50K_R47K_52C       776
#define NTC50K_R47K_53C       784
#define NTC50K_R47K_55C       792
#define NTC50K_R47K_56C       800
#define NTC50K_R47K_57C       808
#define NTC50K_R47K_58C       815
#define NTC50K_R47K_59C       822
#define NTC50K_R47K_60C       829
#define NTC50K_R47K_61C       836
#define NTC50K_R47K_62C       842
#define NTC50K_R47K_63C       848
#define NTC50K_R47K_65C       854
#define NTC50K_R47K_66C       860
#define NTC50K_R47K_67C       866
#define NTC50K_R47K_68C       871
#define NTC50K_R47K_69C       876
#define NTC50K_R47K_70C       882
#define NTC50K_R47K_71C       886
#define NTC50K_R47K_72C       891
#define NTC50K_R47K_73C       896
#define NTC50K_R47K_75C       900
#define NTC50K_R47K_76C       904
#define NTC50K_R47K_77C       909
#define NTC50K_R47K_78C       912
#define NTC50K_R47K_79C       916
#define NTC50K_R47K_80C       920
#define NTC50K_R47K_81C       924
#define NTC50K_R47K_82C       927
#define NTC50K_R47K_83C       930
#define NTC50K_R47K_85C       933
#define NTC50K_R47K_86C       936

// NTC 100K
#define NTC100K_R100K_00C     232
#define NTC100K_R100K_01C     243
#define NTC100K_R100K_02C     254
#define NTC100K_R100K_03C     266
#define NTC100K_R100K_05C     277
#define NTC100K_R100K_06C     289
#define NTC100K_R100K_07C     301
#define NTC100K_R100K_08C     314
#define NTC100K_R100K_09C     326
#define NTC100K_R100K_10C     339
#define NTC100K_R100K_11C     352
#define NTC100K_R100K_12C     365
#define NTC100K_R100K_13C     378
#define NTC100K_R100K_15C     391
#define NTC100K_R100K_16C     405
#define NTC100K_R100K_17C     418
#define NTC100K_R100K_18C     431
#define NTC100K_R100K_19C     445
#define NTC100K_R100K_20C     458
#define NTC100K_R100K_21C     472
#define NTC100K_R100K_22C     485
#define NTC100K_R100K_23C     499
#define NTC100K_R100K_25C     512
#define NTC100K_R100K_26C     525
#define NTC100K_R100K_27C     538
#define NTC100K_R100K_28C     551
#define NTC100K_R100K_29C     564
#define NTC100K_R100K_30C     577
#define NTC100K_R100K_31C     590
#define NTC100K_R100K_32C     602
#define NTC100K_R100K_33C     614
#define NTC100K_R100K_35C     626
#define NTC100K_R100K_36C     638
#define NTC100K_R100K_37C     650
#define NTC100K_R100K_38C     661
#define NTC100K_R100K_39C     672
#define NTC100K_R100K_40C     683
#define NTC100K_R100K_41C     694
#define NTC100K_R100K_42C     705
#define NTC100K_R100K_43C     715
#define NTC100K_R100K_45C     725
#define NTC100K_R100K_46C     734
#define NTC100K_R100K_47C     744
#define NTC100K_R100K_48C     753
#define NTC100K_R100K_49C     762
#define NTC100K_R100K_50C     771
#define NTC100K_R100K_51C     779
#define NTC100K_R100K_52C     788
#define NTC100K_R100K_53C     796
#define NTC100K_R100K_55C     803
#define NTC100K_R100K_56C     811
#define NTC100K_R100K_57C     818
#define NTC100K_R100K_58C     825
#define NTC100K_R100K_59C     832
#define NTC100K_R100K_60C     839
#define NTC100K_R100K_61C     845
#define NTC100K_R100K_62C     851
#define NTC100K_R100K_63C     857
#define NTC100K_R100K_65C     863
#define NTC100K_R100K_66C     868
#define NTC100K_R100K_67C     874
#define NTC100K_R100K_68C     879
#define NTC100K_R100K_69C     884
#define NTC100K_R100K_70C     889
#define NTC100K_R100K_71C     894
#define NTC100K_R100K_72C     898
#define NTC100K_R100K_73C     903
#define NTC100K_R100K_75C     907
#define NTC100K_R100K_76C     911
#define NTC100K_R100K_77C     915
#define NTC100K_R100K_78C     918
#define NTC100K_R100K_79C     922
#define NTC100K_R100K_80C     926
#define NTC100K_R100K_81C     929
#define NTC100K_R100K_82C     932
#define NTC100K_R100K_83C     935
#define NTC100K_R100K_85C     938
#define NTC100K_R100K_86C     941

