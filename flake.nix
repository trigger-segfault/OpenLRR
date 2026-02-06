{
  description = "OpenLRR";

  inputs.nixpkgs.url = github:NixOS/nixpkgs;

  outputs = { self, nixpkgs, ... }: let
    baseVersion = "0.0.0.7.${self.lastModifiedDate}";
    shortRev    = if (self ? shortRev) then self.shortRev else self.dirtyShortRev;
    isDirty     = self ? dirtyShortRev;

    # Replicate a "git describe"
    version = "${baseVersion}-g${shortRev}";
  in {
    packages.x86_64-linux = let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in rec {
      openlrr = pkgs.pkgsCross.mingw32.callPackage ./nix/openlrr.nix {
        inherit version;
      };

      release-zip = pkgs.callPackage ./nix/release-zip.nix {
        openlrr = default;
      };

      default = openlrr;
    };
  };
}
