{ pkgs? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    gcc
    SDL2
    bear
  ];
}
