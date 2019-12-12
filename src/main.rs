use std::net::UdpSocket;
use std::net::SocketAddr;
use std::sync::mpsc;
use std::thread;

const BUFSIZ: usize = 4096;

struct Gram {
    src_addr: SocketAddr,
    len: usize,
    buf: [u8; BUFSIZ],
}

fn main() -> std::io::Result<()> {
    let socket = UdpSocket::bind("0.0.0.0:7").expect("couldn't bind to address");
    let sockfd = socket.try_clone().expect("couldn't clone the socket");

    // 创建一个通道
    let (trans_sender, trans_recver): (mpsc::Sender<Gram>, mpsc::Receiver<Gram>) = mpsc::channel();

    // 发送线程
    thread::spawn(move || {
        loop {
            let form = trans_recver.recv().unwrap();
            let buf = &form.buf[0..form.len];
            sockfd.send_to(&buf, &form.src_addr).unwrap();
        }
    });

    // 接收器
    loop {
        let mut buf = [0; BUFSIZ];
        let (len, src_addr) = socket.recv_from(&mut buf).expect("Didn't receive data");
        let form = Gram {
            src_addr: src_addr,
            len: len,
            buf: buf,
        };
        trans_sender.send(form).unwrap();
    }
}
