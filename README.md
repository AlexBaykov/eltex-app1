# Analysis for $f_2$
This is a series of scripts intended to use inside ROOT CERN.

This is a series of scripts for ROOT CERN. This is my latest analysis of the process of electron-positron annihilation into f_2
meson. These scripts can be rewritten into an executable file, but for the sake of interactivity they are still in this form.

load.C loads everything into ROOT. 
draw.C should be loaded manually.
getSection.C reads the cross-section file for process it is provided with.
getChains.C creates chain of .root files for further analysis. This is optional if there is no intention to work with chains
manually. Chains are only an intermediate step.
analyzeChain.C pushes chains into the analysis and creates foundation for output histograms.
analyseFile.C extracts a tree from the provided file and analyses it, filling three variables for number of events.
draw.C takes the output of analyseChains and calculates expected number of events and builds hitograms.

The distribution of events in the enrgy region of my interest in in events.pdf
