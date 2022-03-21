
SET buildType=%1
IF "%1"=="" (
	SET buildType="Release"
)

pushd build
pushd %buildType%

:: MSBuild Zaml.sln /p:Configuration=%buildType%
cmake --build .. --config %buildType%

popd

if %ERRORLEVEL% EQU 0 bin\Zaml.exe

popd