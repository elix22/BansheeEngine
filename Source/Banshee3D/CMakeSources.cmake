set(BS_BANSHEE3D_INC_NOFILTER
	"resource.h"
)

set(BS_BANSHEE3D_SRC_NOFILTER
	"BsEditorExec.cpp"
	"BansheeEditorExec.rc"
	"BansheeIcon.ico"	
)

source_group("Header Files" FILES ${BS_BANSHEE3D_INC_NOFILTER})
source_group("Source Files" FILES ${BS_BANSHEE3D_SRC_NOFILTER})

set(BS_BANSHEE3D_SRC
	${BS_BANSHEE3D_INC_NOFILTER}
	${BS_BANSHEE3D_SRC_NOFILTER}
)