add_executable( transaction_serializer main.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

target_link_libraries( transaction_serializer
                       PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_none graphene_utilities graphene_wallet fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

install( TARGETS
   transaction_serializer

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
