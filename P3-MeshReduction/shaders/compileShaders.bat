for %%v in (shaders/source/*) do ("shaders/glslangValidator.exe" -V shaders/source/%%v)
mkdir %1
for %%v in (*.spv) do move %%v %1 >nul