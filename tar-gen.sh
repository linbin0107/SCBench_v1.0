#!/bin/bash

for dir in ./*/
do
  dir=${dir%*/}
  name=${dir##*/}
  tar -czvf ${name}.tar.gz ${name}
done
