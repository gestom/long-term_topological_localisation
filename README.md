## Long-Term Topological Localisation

This repository contains the benchmarking software that was used for our research on long-term localisation, which we presented at the IROS 2014 conference [[1](#references)], and ICRA 2015 Workshop on Long-Term Autonomy [[2](#references)]. 
The software in this repository reproduces experiments described in these two papers. 

### Overview

The method presented in [[1,2](#references)] is using the concept of Frequency Map Enhancement ([FreMEn](https://fremen.uk)) to predict appearance of locations in a hybrid feature-topological map.
Predicting appearance for a given time improves robot localisation in changing environments, see the picture and video below for a detailed explanation.
The long-term observations of a particular image feature visibility (red,centre), are transferred to the spectral domain (left).
Transferring the most prominent spectral components (left, green) to the time domain provides an analytic expression (centre) representing the probability of the feature being visible at a given time (green, centre).
This allows to predict the feature visibility at a time when the robot performs self-localization (blue). In this case, the FreMEn model is applied to all features visible at a given location, which allows to predict its appearance for a specific time. 

[![FreMEn for Visual Localization](https://raw.githubusercontent.com/wiki/gestom/fremen/pics/features.png)](https://www.youtube.com/watch?v=aTr9KD4XMGc"FreMEn for Visual Localization")
<b>Click the picture to see a detailed explanation - make sure you have sound on.</b>


### How to use the software

The best way to start with the software is to reproduce the experiments described in [[1,2](#references)].
You just need to perform a few simple steps:

1. Clone or fork this repository.
1. Download relevant datasets.
1. Compile the package. 
1. Build the spatio-temporal models of individual locations. 
1. Benchmark the individual models.
1. Summarize the results. 

#### Getting the software 

Just fork of clone this repository: `https://github.com/gestom/long-term_topological_localisation.git`.
 
#### Dataset

You need to get a relevant dataset to test with.
There are two datasets that we tested our software with, the [Witham Wharf RGB-D](http://lcas.lincoln.ac.uk/owncloud/shared/datasets) and the [Stromovka-FreLoc](https://drive.google.com/open?id=0B7TY_9FitfdlM3pHYTBLeVNRWFk).
You will need to download them if you want to test the software by repeating the experiments of [[1,2](#references)].
 
#### Compilation 

The project requires <i>openCV</i>, and it was tested with openCV version 2.4.8.
The project compilation is Makefile-based, so simply `cd src` to the <i>src</i> folder and call `make`.
If successful, the <b>bin</b> folder should contain the <b>fremen</b> binary.

#### Build the spatio-temporal models 


#### Benchmark localisation 

#### Evaluate results

###References
1. T.Krajnik, J.P.Fentanes, O.M.Mozos, T.Duckett, J.Ekekrantz, M.Hanheide: <b>[Long-term topological localisation for service robots in dynamic environments using spectral maps.](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2014_IROS.pdf)</b> In proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2014. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2014_IROS.bib)]
2. T.Krajnik, J.P.Fentanes, J.Santos, K.Kusumam, T.Duckett: <b>[FreMEn: Frequency Map Enhancement for Long-Term Mobile Robot Autonomy in Changing Environments.](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2015_ICRA_VPRCE.pdf)</b> In proceedings of the ICRA Workshop on Visual Place Recognition in Changing Environments (VPRiCE), 2014. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2015_ICRA_VPRCE.bib)]
