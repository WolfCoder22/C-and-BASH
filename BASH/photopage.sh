#!/bin/bash
#check input paramters
#param1= new directory name
#param2= name of photo album directory in photos diorectory which is in the global cs50 folder
#param3= template dirctory name (should be templates only)

#get last para of param 2 and 3
param2=${2##/*/}
param3=${3##/*/}

path=$(pwd)

if [ "$#" != 3 ]
	then echo "Please have three input paraterms like this: ./photopage.sh newDirectoryName photoDirectoryName templateDirectoryName" 

else

	#see if photo directory is a valid directory
	if (!(test -d /net/class/cs50/data/Lab1/photos/"$param2"))
		then
  			echo "Photo Directory, "$param2", does not exist. Please enter a photo album  directory in /net/class/cs50/data/Lab1/photos/albumName"
  			exit
	fi;

	#see if template directory exists
	if (!(test -d /net/class/cs50/data/Lab1/"$param3"))
		then
  			echo "Template directory does not exist. Please use directory name 'templates'"
  			exit
	fi;

	#all input parameters are working so create new directory with directory name
	if [ -d "$1" ]; then

        
        echo "Directory "$1"  already esits in path "$path" could not be made"
    
    #newdir doesn't exists already
    else
        mkdir "$1"
        mkdir "$1"/images
        mkdir "$1"/thumbs

        #get the title and headcation of image album
        title=$( cat /net/class/cs50/data/Lab1/photos/"$param2"/title.txt )

        #if title couldn't be found
        if [ $? == 1 ]; then
              title='Photo Album'
        fi;

        headcaption=$( cat /net/class/cs50/data/Lab1/photos/"$param2"/header.txt )

        #if headcaption couldn't be found
        if [ $? == 1 ]; then
              headcaption='Photos'
        fi;

        #move more templates to the local dir
        cp -T -f /net/class/cs50/data/Lab1/"$param3"/index.template $path/"$1"/indexplace.html
        cp -T -f /net/class/cs50/data/Lab1/"$param3"/header.template $path/"$1"/headerplace.html
        cp -T -f /net/class/cs50/data/Lab1/"$param3"/empty.template $path/$1/empty.html
        cp -T -f /net/class/cs50/data/Lab1/"$param3"/navigation.bottom.template $path/"$1"/navigation.bottom.template

        #add hml top to new html fie navigation 
        cat /net/class/cs50/data/Lab1/"$param3"/'navigation.top.template' >  $path/"$1"/navigation.html
        
        #chmod the new hmtl files to allow for editing
        chmod 777 "$path"/"$1"/navigation.html

        #update files with data
        cat "$path"/"$1"/indexplace.html | sed -e "s/@TITLE@/$title/g" >> "$path"/"$1"/index.html 

        cat "$path"/"$1"/headerplace.html | sed -e "s/@TITLE@/$title/g" | sed -e "s/@HEADCAPTION@/$headcaption/g" >> "$path"/"$1"/header.html 

        #remove unwanted html placeholder files
        rm -f "$path/$1"/indexplace.html
        rm -f "$path/$1"/headerplace.html


        #go through all photos in directory
        for i in /net/class/cs50/data/Lab1/photos/"$param2"/*.jpg; do
            
            #get image file name and change ext to html
            imagefile=${i##/*/}
            imageNameHTML=${imagefile%.jpg}'.html'

            #move navigation template for editing
            cp -d -f /net/class/cs50/data/Lab1/"$param3"/navigation.item.template "$path/$1"/navigation.item.template
            
            # add photo to image iderctory
            cp  $i "$path/$1"/images/$imagefile

            # #create thumbnail and add it to the thumbnail dir
            convert $i -resize '100x100' "$path/$1"/thumbs/${i##/*/}

            caption=$(exif --ifd=0 --tag=ImageDescription $i --machine-readable 2>/dev/null)

            #check if the caption name was found
            if [ $? == 1 ]; then
                  caption='Blank'
            fi;

            DATE=$(exif --ifd=EXIF --tag=DateTimeOriginal $i --machine-readable 2>/dev/null)

            #check if the Date name was found
            if [ $? == 1 ]; then
                  DATE='unknown'
            fi;

            credit=$(exif --ifd=0 --tag=Artist $i --machine-readable 2>/dev/null)

            #check if the artist name was found
            if [ $? == 1 ]; then
                  credit='unknown'
            fi;

            copyright=$(exif --ifd=0 --tag=Copyright $i --machine-readable 2>/dev/null)

            #check if the copyright name was found
            if [ $? == 1 ]; then
                  copyright='unknown'
            fi;

            #get the image html file for each image
            cp -r -f /net/class/cs50/data/Lab1/"$param3"/image.template "$path/$1"/$imageNameHTML

            sed -i "s/@IMAGEFILE@/$imagefile/g" /"$path/$1"/$imageNameHTML
            sed -i "s/@CAPTION@/$caption/g" "$path/$1"/$imageNameHTML
            sed -i "s/@DATE@/$DATE/g" "$path/$1"/$imageNameHTML
            sed -i "s/@CREDIT@/$credit/g" "$path/$1"/$imageNameHTML
            sed -i "s/@COPYRIGHT@/$copyright/g" "$path/$1"/$imageNameHTML

            #get the data for nav elements
            imagepage="$imageNameHTML"
            thumb="$imagefile"

            #move nav itm and bottom html to fi
            cat "$path/$1"/navigation.item.template | sed -e "s/@IMAGEPAGE@/$imagepage/g"| sed -e "s/@THUMB@/$thumb/g" | sed -e "s/@CAPTION@/$caption/g"  >> "$1"/navigation.html

            #remove navigate template
            rm -f "$path/$1"/navigation.item.template
             
        done

        #append bottom html then remove it
        cat "$path/$1"/navigation.bottom.template >> "$path/$1"/navigation.html
        rm -f "$path/$1"/navigation.bottom.template
    fi

fi
