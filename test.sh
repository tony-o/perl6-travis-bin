#!/bin/bash

perl6 -v >/dev/null

if [ $? -ne 0 ]; then
  echo 'Failure';
  exit 1
else
  echo 'Success';
  exit 0;
fi
