mod process;
mod scheduler;

use process::PCB;
use scheduler::{Algorithm, Scheduler};

fn main() {
    let processes = vec![
        PCB::new(1, 4, 0),
        PCB::new(2, 5, 2),
        PCB::new(3, 4, 3),
        PCB::new(4, 3, 3),
        PCB::new(5, 1, 4),
        PCB::new(6, 2, 5),
    ];

    let mut fcfs_scheduler = Scheduler::new(processes.clone(), Algorithm::FCFS, 0);
    let mut sjf_scheduler = Scheduler::new(processes.clone(), Algorithm::SJF, 0);
    let mut rr_scheduler = Scheduler::new(processes.clone(), Algorithm::RR, 3);

    let mut exited = false;
    while !exited {
        println!("\nwhich algorithm do you wanna run?\n 1: FCFS\n 2: SJF\n 3: RR\n 0: exit simulation");
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).expect("input failed");
        match input.trim() {
            "1" => {
                fcfs_scheduler.run();
            }
            "2" => {
                sjf_scheduler.run();
            }
            "3" => {
                rr_scheduler.run();
            }
            "0" => {
                println!("goodbye.");
                exited = true;
            }
            _ => {
                println!("invalid input");
            }
        }
    }
}