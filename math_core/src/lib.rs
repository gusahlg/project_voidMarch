pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
// Your additions (no_mangle gets red, "unsafe attribute used without unsafe<-|usage of unsafe attribute"):
#[no_mangle]                 // keep symbol name exactly "add_vectors"
pub extern "C" fn add_vectors(a_x: f32, a_y: f32,
                              b_x: f32, b_y: f32,
                              out_x: *mut f32, out_y: *mut f32) {
    unsafe {
        *out_x = a_x + b_x;
        *out_y = a_y + b_y;
    }
}
