{ stdenv
, version
, cmake
, windows
}:
stdenv.mkDerivation(finalAttrs: {
  pname = "openlrr";
  version = "v${version}";

  src = ../.;

  nativeBuildInputs = [
    cmake
  ];

  cmakeFlags = [
    "-DLEGORR_DIR=${placeholder "out"}"
  ];

  postBuild = ''
    cp ${windows.mcfgthreads}/bin/libmcfgthread-2.dll $out
  '';

  dontInstall = true;
  dontStrip = true;
})
