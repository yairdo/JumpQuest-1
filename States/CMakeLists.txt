target_include_directories (OPP_FinalProject_2nd_semester PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_sources (OPP_FinalProject_2nd_semester PRIVATE
                                     "MenuState.cpp"
                                     "MainMenuState.cpp"
                                     "MultiplayerMenuState.cpp"
                                     "StandardMenuState.cpp"
                                     "State.cpp"
                                     "GameState.cpp"
                                     "StateManager.cpp")
