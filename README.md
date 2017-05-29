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

### Automated benchmark

This repository allows for automated benchmarking using the approach described in [[3](#references)]. If you would like to quickly test any improvements of our method, for the repository, do your changes in the code and open a pull request. A Jenkins server will automatically compile your code, compare it against the method presented in [[1,2](#references)] and send you a detailed report.


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
There are two datasets that we tested our software with, the [Witham Wharf RGB-D](http://lcas.lincoln.ac.uk/owncloud/shared/datasets) and the [Stromovka-FreLoc](https://drive.google.com/open?id=0B7TY_9FitfdlNEtYdTJHd0VJNm8).
You will need to download them if you want to test the software by repeating the experiments of [[1,2](#references)].
For quick testing, download the [Stromovka-FreLoc](https://drive.google.com/open?id=0B7TY_9FitfdlNEtYdTJHd0VJNm8) and unzip it in the <i>src</i> folder.
 
#### Compilation 

The project requires <i>openCV</i>, and it was tested with openCV version 2.4.8.
The project compilation is Makefile-based, so simply `cd src` to the <i>src</i> folder and call `make`.
If successful, the <b>bin</b> folder should contain the <b>fremen</b> binary.
To adjust to the partular dataset, change the <i>timeQuantum</i> variable in <b>main/fremen.cpp</b>.

#### Build the spatio-temporal models 

To build a feature-based model of a particular location, you need to 

1. Build a set of features visible at a given location. This is done by calling the <b>fremen</b> binary with the <i>build</i> argument, e.g.: `../bin/fremen build stromovka/training/place_0/ stromovka/maps/place_0.all` takes all the images in the <i>stromovka/training/place_0/</i> and creates a feature-based map of that location in <i>stromovka/maps/place_0.all</i>.
1. Remove unstable features from the build map. This is done by calling the <b>fremen</b> binary with the <i>reduce</i> argument, e.g.: `../bin/fremen reduce stromovka/maps/place_0.all 0.05 stromovka/maps/place_0.red` removes features that were visible in less than 5% of images.
1. Rebuild the temporal models of features' visibility. This is done by calling the <b>fremen</b> binary with the <i>recalculate</i> argument, e.g.: `../bin/fremen recalculate stromovka/training/place_0/ stromovka/maps/place_0.red stromovka/maps/place_0.map` registers the features of images stored in <i> stromovka/training/place_0/</i> with the map in <i>stromovka/maps/place_0.red</i> and stores their re-detections in <i>stromovka/maps/place_0.map</i>.

Alternatively, you can simply call the <i>scripts/buildmaps.sh FOLDER</i>, which performs the three aforementioned steps for all locations stored in <i>FOLDER/traninng</i>, i.e.

``./scripts/buildmaps.sh stromovka``

builds the maps, removes unstable features and creates FreMEN visibility models of all images arranged in <i>stromovka/training/place_X</i> folders.

#### Benchmark localisation 

To run the benchmarks, run the <b>fremen</b> with the <i>test</i> argument. The other arguments are <i>image folder</i>, <i>map_folder</i>, <i>num_features</i>, and <i>fremen_order</i>  e.g.:

``../bin/fremen test stromovka/testing/ stromovka/maps/ 15 1``

tries to perform the localisation using 15 most likely visible features generates by FreMEn order 1 from temporal models stored in <i>stromovka/maps</i> and images stored in <i>stromovka/testing</i>.
To perform the test for all FreMEn orders and 1-50 feature numbers, simply call the available <b>test.sh</b> script with the dataset folder as an parameter, e.g.:

``./scripts/test.sh stromovka/``.

This will fill the <i>stromovka/reports</i> folder with resuts of the localisation.

#### Evaluate results

Finally, you can call the script 

``./scripts/summarize.sh stromovka/`` 

and the <i>stromovka/summary</i> folder will contain the results of the localization for different FreMEn orders in text format and as a graph in <i>xfig</i>, <i>png</i> and <i>pdf</i>.
The graph <i>stromovka/summary/error.pdf</i> should look like the Figure 3 of [[2](#references)].

###References
1. T.Krajnik, J.P.Fentanes, O.M.Mozos, T.Duckett, J.Ekekrantz, M.Hanheide: <b>[Long-term topological localisation for service robots in dynamic environments using spectral maps.](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2014_IROS.pdf)</b> In proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2014. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2014_IROS.bib)]
2. T.Krajnik, J.P.Fentanes, J.Santos, K.Kusumam, T.Duckett: <b>[FreMEn: Frequency Map Enhancement for Long-Term Mobile Robot Autonomy in Changing Environments.](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2015_ICRA_VPRCE.pdf)</b> In proceedings of the ICRA Workshop on Visual Place Recognition in Changing Environments (VPRiCE), 2014. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/fremen/papers/fremen_2015_ICRA_VPRCE.bib)]
