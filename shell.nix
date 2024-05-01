with import <nixpkgs> { };

mkShell {

  # Package names can be found via https://search.nixos.org/packages
  nativeBuildInputs = [
    SDL2
    cmake
  ];

  NIX_ENFORCE_PURITY = true;

  shellHook = ''
  '';
}
