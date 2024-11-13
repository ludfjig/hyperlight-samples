use http_body_util::BodyExt;
use hyper::body;
use hyperlight_host::sandbox_state::sandbox::EvolvableSandbox;
use hyperlight_host::sandbox_state::transition::Noop;
use hyperlight_host::GuestBinary;
use hyperlight_host::MultiUseSandbox;
use hyperlight_host::UninitializedSandbox;

use std::convert::Infallible;
use std::net::SocketAddr;

use http_body_util::Full;
use hyper::body::Bytes;
use hyper::server::conn::http1;
use hyper::service::service_fn;
use hyper::{Request, Response};
use hyper_util::rt::TokioIo;
use tokio::net::TcpListener;

// fn main() {
//     let guest_binary = GuestBinary::FilePath("../quickjs_guest/a.out".to_string());
//     let sandbox = UninitializedSandbox::new(guest_binary, None, None, None).unwrap();
//     let mut multiusesandbox: MultiUseSandbox = sandbox.evolve(Noop::default()).unwrap();
//     multiusesandbox
//         .call_guest_function_by_name("GuestMethod1", hyperlight_host::func::ReturnType::Int, None)
//         .unwrap();
// }

async fn hello(req: Request<hyper::body::Incoming>) -> Result<Response<Full<Bytes>>, Infallible> {
    // get the body of the request as a vec<u8>
    let bytes = req.collect().await.unwrap().to_bytes();
    let vec: Vec<u8> = bytes.into();

    Ok(Response::new(Full::new(Bytes::from_static(
        b"Hello, World!",
    ))))
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error + Send + Sync>> {
    let addr = SocketAddr::from(([127, 0, 0, 1], 3000));

    // We create a TcpListener and bind it to 127.0.0.1:3000
    let listener = TcpListener::bind(addr).await?;
    println!("Listening on {}...", addr);

    // We start a loop to continuously accept incoming connections
    loop {
        let (stream, _) = listener.accept().await?;

        // Use an adapter to access something implementing `tokio::io` traits as if they implement
        // `hyper::rt` IO traits.
        let io = TokioIo::new(stream);

        // Spawn a tokio task to serve multiple connections concurrently
        tokio::task::spawn(async move {
            // Finally, we bind the incoming connection to our `hello` service
            if let Err(err) = http1::Builder::new()
                // `service_fn` converts our function in a `Service`
                .serve_connection(io, service_fn(hello))
                .await
            {
                eprintln!("Error serving connection: {:?}", err);
            }
        });
    }
}
