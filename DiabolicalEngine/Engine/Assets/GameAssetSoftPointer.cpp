#include "GameAssetSoftPointer.h"
GameAsset* SoftPointer::Get_Internal()
{
	GameAsset* LoadedAsset = GameAsset::GetIfLoaded(Path);

	//if (LoadedAsset)
	//{
	//	LOGVERBOSE("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", already loaded");
	//}
	//else
	//{
	//	LOGVERBOSE("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", not loaded");
	//}
	return LoadedAsset;
}

const std::filesystem::path& SoftPointer::GetPath()
{
	return Path;
}

const std::string SoftPointer::GetFileExtension()
{
	return Path.extension().string();
}