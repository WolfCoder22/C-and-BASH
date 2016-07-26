#!/bin/bash
if [ "$#" == 0 ];
        then echo "Please enter the name of the artists to search for as the first arguement"
        else

        #make sure have name of artist
        if [[ $1 == *"."* ]];
                then echo "Make first argument an artist's name not file"

        else
                echo ""
                echo "Searching for artist $1"
                echo ""

                count=1;
                #loop through all files after first arguent
                for i in "$@"
                do
                        #isn't the artist parameter check the tag       
                        if [ $count != 1 ]; then
                                exif --ifd=0 --tag=Artist  $i --machine-readable 2>/dev/null | grep -q "$1" 2>/dev/null

                                #check if the artist name was found
                                if [ $? == 0 ]; then
                                        echo "$i";
                                fi;
                        fi;

                ((count++));
                done;
          
        fi;
fi;       
