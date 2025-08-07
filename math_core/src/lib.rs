//! math_core — Rust helper library linked into the C++ / Raylib game.
//! Build type is `staticlib` (set in Cargo.toml), so the C++ linker sees a .lib.

#[no_mangle]                          // exact symbol name "add_vectors"
pub extern "C" fn add_vectors(
    a_x: f32, a_y: f32,
    b_x: f32, b_y: f32,
    out_x: *mut f32,
    out_y: *mut f32,
) {
    if out_x.is_null() || out_y.is_null() {
        return;                       // null-safety guard
    }

    // SAFETY: out_x/out_y are proven non-null; caller owns the memory.
    unsafe {
        *out_x = a_x + b_x;
        *out_y = a_y + b_y;
    }
}

// ---- Pure-Rust helpers & unit test ----

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn add_vectors_works() {
        let mut x = 0.0;
        let mut y = 0.0;
        unsafe { add_vectors(1.0, 2.0, 3.0, 4.0, &mut x, &mut y) };
        assert_eq!((x, y), (4.0, 6.0));
    }
}
