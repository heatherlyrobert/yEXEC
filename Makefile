#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = yEXEC
# additional standard and outside libraries
LIB_STD    = -lm
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lySTR_debug -lcrypt
# directory for production code, no trailing slash
INST_DIR   = 



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
install_post       :

#remove_post        :



#================================[[ end-code ]]================================#
