# 승격 전에는 헤더가 비어도 괜찮. 비어도 타겟은 만들어 둠.
add_library(toolbox INTERFACE)
target_include_directories(toolbox INTERFACE include)