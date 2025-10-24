{
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/2d2a9ddbe3f2c00747398f3dc9b05f7f2ebb0f53"; # nixpkgs-unstable as of 2024-10-31T18:10+01:00
	};

	outputs = { self, nixpkgs }@inputs:
		let
			system = "x86_64-linux";
			pkgs = nixpkgs.legacyPackages.${system};
		in {
			devShells.${system}.default = pkgs.mkShell {
				nativeBuildInputs = with pkgs; [
					gcc
					gnumake
					glfw
					pkg-config
					vulkan-loader
					vulkan-headers
				];
			};
		};
}

