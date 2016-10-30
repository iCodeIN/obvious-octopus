# variable declarations
BOOST='/home/joris/Downloads/boost_1_60_0'
MYDIR='/home/joris/Code/cdev_boost/'

EXEC_OUT_NAME='GraphLayout'
DOXY_OUT_NAME='_htmldoc'
OBJS_OUT_NAME='_objects'

COMP_LOG_NAME='comp.log'
LINK_LOG_NAME='link.log'
DOXY_LOG_NAME='doxy.log'

# remove all old object files
if [[ -d $OBJS_OUT_NAME ]]; then
	rm _objects/*.o
fi

# remove logs
if [[ -f $COMP_LOG_NAME ]]; then
	rm $COMP_LOG_NAME
fi
if [[ -f $LINK_LOG_NAME ]]; then
	rm $LINK_LOG_NAME
fi
if [[ -f $DOXY_LOG_NAME ]]; then
	rm $DOXY_LOG_NAME
fi

# zip the content of the directory into an archive (in case anything goes wrong)
# zip -r "backup_$(date +"%m_%d_%Y")" .

# apply code formatting to all files
astyle -r -q --style=allman --indent=spaces=4 --indent-classes --indent-switches --indent-cases --indent-namespaces --add-brackets --close-templates --suffix=none *.hpp
astyle -r -q --style=allman --indent=spaces=4 --indent-classes --indent-switches --indent-cases --indent-namespaces --add-brackets --close-templates --suffix=none *.h
astyle -r -q --style=allman --indent=spaces=4 --indent-classes --indent-switches --indent-cases --indent-namespaces --add-brackets --close-templates --suffix=none *.cpp

# iterate over all object in the current folder
for folder in *; do

	# check whether the object is a directory
	if [[ -d $folder ]]; then

		# check whether the name does not start with '_'
		if ! [[ $folder =~ _.* ]]; then

			# beautify all files

			# compile, write errors to log
			if [[ `find $folder -maxdepth 1 -iname *.cpp | wc -l` > 0 ]]; then
				echo "Compiling all files in" $folder
				g++ -std=c++1y -I $BOOST -iquote $MYDIR -g -c $folder/*.cpp 2>> $COMP_LOG_NAME
			else
				echo "No cpp files in" $folder
			fi

		fi	
	fi
done

# move all object files to the directory _objects
echo "Moving all object files"
mv *.o $OBJS_OUT_NAME/

# link
echo "Linking object files"
g++ -std=c++11 -I $BOOST $OBJS_OUT_NAME/*.o -o $EXEC_OUT_NAME 2>> $LINK_LOG_NAME

# print status
if [[ -f $EXEC_OUT_NAME ]]; then
	tput setaf 2
	echo 'Build succeeded.'
	tput sgr0
else
	tput setaf 1
	echo 'Build failed. Check ' $COMP_LOG_NAME ' and ' $LINK_LOG_NAME ' for more details.'
	tput sgr0
fi

# generate doxygen
echo "Generating doxygen"
doxygen default_doxy_config 2> $DOXY_LOG_NAME > /dev/null

# remove logs if they are empty
if ! [[ -s $COMP_LOG_NAME ]]; then
	rm $COMP_LOG_NAME
fi
if ! [[ -s $LINK_LOG_NAME ]]; then
	rm $LINK_LOG_NAME
fi
if ! [[ -s $DOXY_LOG_NAME ]]; then
	rm $DOXY_LOG_NAME
fi