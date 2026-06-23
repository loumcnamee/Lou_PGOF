file(GLOB PGOF_MAIN_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(pgof_main
	${PGOF_MAIN_SOURCES}
)

target_link_libraries(pgof_main
	PRIVATE
		pgof::pgof
)

