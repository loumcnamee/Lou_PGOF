file(GLOB PGOF_MAIN_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(pgof_main
	${PGOF_MAIN_SOURCES}
)

target_link_libraries(pgof_main
	PRIVATE
		pgof::garage_capacity
)

# Ensure the include path for garage_capacity headers is available
target_include_directories(pgof_main
	PRIVATE
		${CMAKE_SOURCE_DIR}/libs/garage_capacity/include
)


