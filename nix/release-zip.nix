{ stdenvNoCC
, zip
, openlrr
}:
stdenvNoCC.mkDerivation(finalAttrs: {
  pname = "openlrr-release-zip";
  version = openlrr.version;

  dontUnpack = true;

  nativeBuildInputs = [
    zip
  ];

  dontBuild = true;

  installPhase = ''
    runHook preInstall

    mkdir -p $out

    pushd ${openlrr}
    zip -9r "$out/openlrr-${openlrr.version}.zip" *
    popd

    runHook postInstall
  '';
})