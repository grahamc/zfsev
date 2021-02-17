let
  pkgs = import <nixpkgs> {};
in
pkgs.stdenv.mkDerivation {
  pname = "zfsev";
  version = "0.0.0";
  nativeBuildInputs = with pkgs; [ pkgconfig cmake ];
  buildInputs = with pkgs; [ zfs utillinux zlib openssl ];
  src = ./src;
}
