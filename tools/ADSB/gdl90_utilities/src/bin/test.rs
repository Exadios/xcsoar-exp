extern crate gdl90;
use gdl90::datalink::Gdl90DatalinkMessage;
use gdl90::Gdl90Message;
use std::fs::File;
use std::io;
use std::io::prelude::*;

/*
 * \Note
 * 2024-010-26.
 *
 * At present this code results in the error "thread 'main' panicked at /home/user/.cargo/registry/src/index.crates.io-6f17d22bba15001f/gdl90-1.0.0/src/datalink.rs:147:29:". This happens apparently when an Ownship Geometric Altitude
 * packet is processed.
 *
 * Logged an issue on this crate's Github repository.
 */
fn main() {

    'read_file: loop {
        let mut bcount = 0;
        let mut flag_found = false;
        let mut packet_terminated = false;
        let mut b: [u8; 1] = [0; 1];
        let mut p: [u8; 256] = [0; 256];
        'read_packet: loop {
            let r = io::stdin().read(&mut b);
            let _n = match r {
                Result::Ok(n) => n,
                Result::Err(error) => panic!("Read error {error:?}"),
            };
            if bcount == 0 && b[0] != 0x7e {
                continue; // Not at the start of packet yet.
            }
            if b[0] == 0x7e {
                if flag_found == false {
                    flag_found = true;
                    bcount = 0;
                } else {
                    packet_terminated = true;
                }
            }
            p[bcount] = b[0];
            // println!("{:#04x}, {}, {}, {}", b[0], flag_found, packet_terminated, bcount);
            bcount += 1;

            if b[0] == 0x7e {
                if bcount == 2 {
                    //
                    p[0] = p[1];
                    bcount = 1;
                    continue;
                }
                if packet_terminated == true {
                    // let result = parse(&[0x7E, 0x00, 0x81, 0x41, 0xDB, 0xD0, 0x08, 0x02, 0xB3, 0x8B, 0x7E, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0]);
            //        dbg!(&p);
                    let result = parse(&[126, 11, 0, 202, 0, 12, 251, 136, 126,]);
                    // let result = parse(&p);

                    println!("{:?}", result);
                    break 'read_packet;
                }
            }
        }
    }
}

fn parse(packet: &[u8]) {
    let parsed = gdl90::read_raw(packet).unwrap();
    //    dbg!(&parsed);

    match parsed.message_data {
        Gdl90DatalinkMessage::Heartbeat {
            status_byte_1: _,
            status_byte_2: _,
            uat_timestamp: _,
            message_counts: _,
        } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::Initialization {
            configuration_byte_1: _,
            configuration_byte_2: _,
        } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::UplinkData {
            time_of_reception: _,
            payload: _,
        } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::HeightAboveTerrain { hat: _ } => {
            println!("{:?}", parsed.message_data)
        }
        Gdl90DatalinkMessage::OwnshipReport { report: _ } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::TrafficReport { report: _ } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::OwnshipGeoometricAltitude {
            ownship_geo_altitude: _,
            vertical_metrics: _,
        } => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::BasicReport() => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::LongReport() => println!("{:?}", parsed.message_data),
        Gdl90DatalinkMessage::Unknown => println!("Unknown"),
    }
    match parsed.message_data {
        Gdl90DatalinkMessage::Heartbeat {
            status_byte_1: _,
            status_byte_2: _,
            uat_timestamp: _,
            message_counts: _,
        } => {}
        Gdl90DatalinkMessage::Initialization {
            configuration_byte_1: _,
            configuration_byte_2: _,
        } => {}
        Gdl90DatalinkMessage::UplinkData {
            time_of_reception: _,
            payload: _,
        } => {}
        Gdl90DatalinkMessage::HeightAboveTerrain { hat: _ } => {}
        Gdl90DatalinkMessage::OwnshipReport { report: _ } => {}
        Gdl90DatalinkMessage::TrafficReport { report: _ } => {}
        Gdl90DatalinkMessage::OwnshipGeoometricAltitude {
            ownship_geo_altitude: _,
            vertical_metrics: _,
        } => {}
        Gdl90DatalinkMessage::BasicReport() => {}
        Gdl90DatalinkMessage::LongReport() => {}
        Gdl90DatalinkMessage::Unknown => {}
    }
}
