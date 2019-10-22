#!/bin/sh

date -v'+5S' '+%s:Meeting in 5 seconds' > TESTDATA
date -v'+1H' '+%s:Meeting in 1 hour' >> TESTDATA
date -v'+2H' '+%s:Meeting in 2 hours' >> TESTDATA
