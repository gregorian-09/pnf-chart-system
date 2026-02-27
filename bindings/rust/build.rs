use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let root_dir = PathBuf::from(manifest_dir).parent().unwrap().parent().unwrap().to_path_buf();

    let lib_dir = if let Ok(dir) = env::var("PNF_LIB_DIR") {
        PathBuf::from(dir)
    } else if cfg!(target_os = "windows") {
        root_dir.join("build").join("bin")
    } else if cfg!(target_os = "macos") {
        root_dir.join("build").join("lib")
    } else {
        let linux_build = root_dir.join("build-linux").join("lib");
        if linux_build.exists() {
            linux_build
        } else {
            root_dir.join("build").join("lib")
        }
    };

    println!("cargo:rustc-link-search=native={}", lib_dir.display());

    println!("cargo:rustc-link-lib=dylib=pnf");
    if cfg!(target_os = "linux") {
        println!("cargo:rustc-link-arg=-Wl,-rpath,{}", lib_dir.display());
    } else if cfg!(target_os = "macos") {
        println!("cargo:rustc-link-arg=-Wl,-rpath,{}", lib_dir.display());
    }

    println!("cargo:rerun-if-changed=build.rs");
    println!("cargo:rerun-if-changed=../c/pnf_c.hpp");
}
