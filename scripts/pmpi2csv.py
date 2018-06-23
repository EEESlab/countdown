#!/usr/bin/env python

#import sys
import os
import re
import struct
import argparse
from mpi4py import MPI

MPI_NONE = -1
MPI_ALL = -2
MPI_ALLV = -3

mpi_call_names = []

max_lenght_64 = pow(2, 64)
max_lenght_48 = pow(2, 48)
GROUP_WORLD = 0

# General Info
def get_gen_info(f_input, input_file, output_dir, no_labels):
    gen_info = {}
    
    buff = f_input.read(128)
    gen_info["hostname"] = buff.strip().split("\0", 1)[0].strip()

    buff = f_input.read(128)
    gen_info["cpu_model_name"] = buff.strip().split("\0", 1)[0].strip()

    buff = f_input.read(4)
    gen_info["HT_capability"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    gen_info["HT_enable"] = struct.unpack("i", buff)[0]

    buff = f_input.read(4)
    gen_info["cpu_id"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    gen_info["process_id"] = struct.unpack("i", buff)[0]

    buff = f_input.read(4)
    gen_info["mpi_rank"] = struct.unpack("i", buff)[0]

    buff = f_input.read(8)
    gen_info["group_count"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    gen_info["comm_count"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    gen_info["call_count"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    gen_info["tsc_start"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    gen_info["tsc_end"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    gen_info["epoch_start"] = struct.unpack("d", buff)[0]
    
    buff = f_input.read(8)
    gen_info["epoch_end"] = struct.unpack("d", buff)[0]

    buff = f_input.read(4)
    gen_info["no_fixed"] = struct.unpack("i", buff)[0]

    buff = f_input.read(4)
    gen_info["no_pmu"] = struct.unpack("i", buff)[0]

    buff = f_input.read(4)
    gen_info["num_mpi_call"] = struct.unpack("i", buff)[0]
    
    for i in range(gen_info["num_mpi_call"]):
        buff = f_input.read(4)
        str_len = struct.unpack("i", buff)[0]
        buff = f_input.read(str_len)
        mpi_call_names.append(buff.strip().split("\0", 1)[0])
        
    gen_info["real_nom_freq"] = diff_64(gen_info["tsc_start"], gen_info["tsc_end"]) / \
                       (gen_info["epoch_end"] - gen_info["epoch_start"])
        
    # Write general info file
    output_file = output_dir + "/" + input_file.split("/")[-1].split(".")[0] + ".info.csv"
    with open(output_file, "w") as f_output:
        # Labels
        if not no_labels:
            f_output.write("hostname;cpu_model_name;HT_capability;HT_enable;"
                           "cpu_id;real_nom_freq;process_id;mpi_rank;"
                           "group_count;comm_count;call_count;"
                           "tsc_start;tsc_end;epoch_start;epoch_end\n")
        
        # Data
        f_output.write("%s;%s;%d;%d;%d;%d;%d;%d;%lu;%lu;%lu;%lu;%lu;%.9f;%.9f\n" %
                       (gen_info["hostname"],
                        gen_info["cpu_model_name"],
                        gen_info["HT_capability"],
                        gen_info["HT_enable"],
                        gen_info["cpu_id"],
                        gen_info["real_nom_freq"],
                        gen_info["process_id"],
                        gen_info["mpi_rank"],
                        gen_info["group_count"],
                        gen_info["comm_count"],
                        gen_info["call_count"],
                        gen_info["tsc_start"],
                        gen_info["tsc_end"],
                        gen_info["epoch_start"],
                        gen_info["epoch_end"]))
        
    return gen_info

# Groups
def get_groups(f_input, gen_info, input_file, output_dir, no_labels):
    groups = []
    group_world_size = 0
    for i in range(gen_info["group_count"]):
        group_info = {}
        
        buff = f_input.read(4)
        group_info["idx"] = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        group_info["size"] = struct.unpack("i", buff)[0]
        if i == 0: group_world_size = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        group_info["deallocated"] = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        group_info["local_rank"] = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        group_info["world_rank"] = struct.unpack("i", buff)[0]
        
        world_ranks = []
        for _ in range(group_world_size):
            buff = f_input.read(4)
            world_ranks.append(struct.unpack("i", buff)[0])
        
        group_info["world_ranks"] = world_ranks
        groups.append(group_info)
        
        # Write group file
        output_file = output_dir + "/" + \
                      input_file.split("/")[-1].split(".")[0] + \
                      ".group.csv"
        with open(output_file, "w") as f_output:
            # Labels
            if not no_labels:
                f_output.write("idx;size;deallocated;local_rank;world_rank")
                for i in range(group_world_size):
                    f_output.write(";proc_%d" % i)
                f_output.write("\n")
            
            # Data
            for g in groups:
                f_output.write("%d;%d;%d;%d;%d" % (g["idx"], 
                                                   g["size"], 
                                                   g["deallocated"], 
                                                   g["local_rank"], 
                                                   g["world_rank"]))
                for j in range(group_world_size):
                    f_output.write(";%d" % g["world_ranks"][j])
                f_output.write("\n")
                
    return groups
  
# Communicators
def get_comms(f_input, gen_info, input_file, output_dir, no_labels):
    comms = []
    for i in range(gen_info["comm_count"]):
        comm_info = {}
        
        buff = f_input.read(4)
        comm_info["comm_idx"] = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        comm_info["group_idx"] = struct.unpack("i", buff)[0]
        
        buff = f_input.read(4)
        comm_info["deallocated"] = struct.unpack("i", buff)[0]
        
        comms.append(comm_info)
        
    # Write group finput_fileile
    output_file = output_dir + "/" + \
                  input_file.split("/")[-1].split(".")[0] + \
                  ".comm.csv"
    with open(output_file, "w") as f_output:
        # Labels
        if not no_labels:
            f_output.write("comm_idx;group_idx;deallocated\n")
        
        # Data
        for c in comms:
            f_output.write("%d;%d;%d\n" % (c["comm_idx"], 
                                           c["group_idx"], 
                                           c["deallocated"]))
            
    return comms

def get_next_mpi_call(f_input, gen_info):
    call_info = {}
        
    buff = f_input.read(8)
    call_info["idx"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(4)
    call_info["type"] = mpi_call_names[struct.unpack("i", buff)[0]]
    
    buff = f_input.read(4)
    call_info["comm_idx"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    call_info["group_idx"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(8)
    call_info["tsc_start"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(8)
    call_info["tsc_end"] = struct.unpack("L", buff)[0]

    buff = f_input.read(8)
    call_info["epoch_start"] = struct.unpack("d", buff)[0]
    
    buff = f_input.read(8)
    call_info["epoch_end"] = struct.unpack("d", buff)[0]
    
    if gen_info["no_fixed"] == 0:
        buff = f_input.read(8)
        call_info["fixed_1_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["fixed_1_end"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["fixed_2_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["fixed_2_end"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["fixed_3_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["fixed_3_end"] = struct.unpack("L", buff)[0]
    
    if gen_info["no_pmu"] == 0:
        buff = f_input.read(8)
        call_info["pmu_1_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_1_end"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_2_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_2_end"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_3_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_3_end"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_4_start"] = struct.unpack("L", buff)[0]
        
        buff = f_input.read(8)
        call_info["pmu_4_end"] = struct.unpack("L", buff)[0]
        
        if gen_info["HT_enable"] == 0:
            buff = f_input.read(8)
            call_info["pmu_5_start"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_5_end"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_6_start"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_6_end"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_7_start"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_7_end"] = struct.unpack("L", buff)[0]

            buff = f_input.read(8)
            call_info["pmu_8_start"] = struct.unpack("L", buff)[0]
            
            buff = f_input.read(8)
            call_info["pmu_8_end"] = struct.unpack("L", buff)[0]
    
    buff = f_input.read(4)
    call_info["dest"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    call_info["send_size"] = struct.unpack("i", buff)[0]

    if call_info["dest"] == MPI_ALLV:
        call_info["send_count"] = []
        
        group_size = groups[call_info["group_idx"]]["size"]
        for j in range(group_size):
            buff = f_input.read(4)
            call_info["send_count"].append(struct.unpack("i", buff)[0])
    else:
        buff = f_input.read(4)
        call_info["send_count"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    call_info["source"] = struct.unpack("i", buff)[0]
    
    buff = f_input.read(4)
    call_info["recv_size"] = struct.unpack("i", buff)[0]
    
    if call_info["source"] == MPI_ALLV:
        call_info["recv_count"] = []
        group_size = groups[call_info["group_idx"]]["size"]
        for j in range(group_size):
            buff = f_input.read(4)
            call_info["recv_count"].append(struct.unpack("i", buff)[0])
    else:
        buff = f_input.read(4)
        call_info["recv_count"] = struct.unpack("i", buff)[0]

    return call_info

def diff_64(start, end):
    if end >= start:
        return end - start
    else:
        return (max_lenght_64 - start) + end
    
def diff_48(start, end):
    if end >= start:
        return end - start
    else:
        return (max_lenght_48 - start) + end
    
def get_ref_tsc(value, gen_info):
    if value >= gen_info["tsc_start"]:
        return value - gen_info["tsc_start"]
    else:
        return (max_lenght_64 - gen_info["tsc_start"]) + value
    
def world_rank_2_local_rank(world_rank, group):
    if group["world_ranks"][world_rank] >= 0:
        return group["world_ranks"][world_rank]
    else:
        return -1
        
# Calls
def get_mpi_calls(f_input, gen_info, groups, comms, input_file, output_dir, no_labels):
    #print input_file, output_dir, no_labels
    output_file = output_dir + "/" + input_file.split("/")[-1].split(".")[0] + ".call.csv"
    f = open(output_file, "w")
    
    # Labels
    if not no_labels:
        f.write("idx;app_tsc_start;app_tsc_end;"
                "app_time_start;app_time_end")
        if gen_info["no_fixed"] == 0:
            f.write(";app_fixed_1;app_fixed_2;app_fixed_3")
        if gen_info["no_pmu"] == 0:
            f.write(";app_pmu_1;app_pmu_2;app_pmu_3;app_pmu_4")
            if gen_info["HT_enable"] == 0:
                f.write(";app_pmu_5;app_pmu_6;app_pmu_7;app_pmu_8")
        
        f.write(";mpi_type;mpi_comm_idx;mpi_group_idx;"
                "mpi_tsc_start;mpi_tsc_end;"
                "mpi_time_start;mpi_time_end")
        if gen_info["no_fixed"] == 0:
            f.write(";mpi_fixed_1;mpi_fixed_2;mpi_fixed_3")
        if gen_info["no_pmu"] == 0:
            f.write(";mpi_pmu_1;mpi_pmu_2;mpi_pmu_3;mpi_pmu_4")
            if gen_info["HT_enable"] == 0:
                f.write(";mpi_pmu_5;mpi_pmu_6;mpi_pmu_7;mpi_pmu_8")
            
        for i in range(groups[GROUP_WORLD]["size"]):
            f.write(";mpi_dest_proc_%d" % i)
        for i in range(groups[GROUP_WORLD]["size"]):
            f.write(";mpi_source_proc_%d" % i)
        f.write("\n")
    
    # Data
    call = [None, None]
    call[1] = get_next_mpi_call(f_input, gen_info)
    for i in range(gen_info["call_count"]-1):
        call[0] = call[1].copy()
        call[1] = get_next_mpi_call(f_input, gen_info)
        
        tsc_start_app = get_ref_tsc(call[0]["tsc_end"], gen_info)
        tsc_end_app = get_ref_tsc(call[1]["tsc_start"], gen_info)
        time_start_app = call[0]["epoch_end"]
        time_end_app = call[1]["epoch_start"]
        if gen_info["no_fixed"] == 0:
            fixed_1_app = diff_48(call[0]["fixed_1_end"], call[1]["fixed_1_start"])
            fixed_2_app = diff_48(call[0]["fixed_2_end"], call[1]["fixed_2_start"])
            fixed_3_app = diff_48(call[0]["fixed_3_end"], call[1]["fixed_3_start"])
        if gen_info["no_pmu"] == 0:
            pmu_1_app = diff_48(call[0]["pmu_1_end"], call[1]["pmu_1_start"])
            pmu_2_app = diff_48(call[0]["pmu_2_end"], call[1]["pmu_2_start"])
            pmu_3_app = diff_48(call[0]["pmu_3_end"], call[1]["pmu_3_start"])
            pmu_4_app = diff_48(call[0]["pmu_4_end"], call[1]["pmu_4_start"])
            if gen_info["HT_enable"] == 0:
                pmu_5_app = diff_48(call[0]["pmu_5_end"], call[1]["pmu_5_start"])
                pmu_6_app = diff_48(call[0]["pmu_6_end"], call[1]["pmu_6_start"])
                pmu_7_app = diff_48(call[0]["pmu_7_end"], call[1]["pmu_7_start"])
                pmu_8_app = diff_48(call[0]["pmu_8_end"], call[1]["pmu_8_start"])
        
        tsc_start_mpi = get_ref_tsc(call[1]["tsc_start"], gen_info)
        tsc_end_mpi = get_ref_tsc(call[1]["tsc_end"], gen_info)
        time_start_mpi = call[1]["epoch_start"]
        time_end_mpi = call[1]["epoch_end"]
        if gen_info["no_fixed"] == 0:
            fixed_1_mpi = diff_48(call[1]["fixed_1_start"], call[1]["fixed_1_end"])
            fixed_2_mpi = diff_48(call[1]["fixed_2_start"], call[1]["fixed_2_end"])
            fixed_3_mpi = diff_48(call[1]["fixed_3_start"], call[1]["fixed_3_end"])
        if gen_info["no_pmu"] == 0:
            pmu_1_mpi = diff_48(call[1]["pmu_1_start"], call[1]["pmu_1_end"])
            pmu_2_mpi = diff_48(call[1]["pmu_2_start"], call[1]["pmu_2_end"])
            pmu_3_mpi = diff_48(call[1]["pmu_3_start"], call[1]["pmu_3_end"])
            pmu_4_mpi = diff_48(call[1]["pmu_4_start"], call[1]["pmu_4_end"])
            if gen_info["HT_enable"] == 0:
                pmu_5_mpi = diff_48(call[1]["pmu_5_start"], call[1]["pmu_5_end"])
                pmu_6_mpi = diff_48(call[1]["pmu_6_start"], call[1]["pmu_6_end"])
                pmu_7_mpi = diff_48(call[1]["pmu_7_start"], call[1]["pmu_7_end"])
                pmu_8_mpi = diff_48(call[1]["pmu_8_start"], call[1]["pmu_8_end"])
            
        app_info = "%lu;%lu;%lu;%.9f;%.9f" % (call[1]["idx"]-1, \
                    tsc_start_app, tsc_end_app, time_start_app, time_end_app)
        if gen_info["no_fixed"] == 0:
            app_info += ";%lu;%lu;%lu" % (fixed_1_app, fixed_2_app, fixed_3_app)
        if gen_info["no_pmu"] == 0:
            app_info += ";%lu;%lu;%lu;%lu" % (pmu_1_app, pmu_2_app, pmu_3_app, pmu_4_app)
            if gen_info["HT_enable"] == 0:
                app_info += ";%lu;%lu;%lu;%lu" % (pmu_5_app, pmu_6_app, pmu_7_app, pmu_8_app)
           
        mpi_info = ";%s;%d;%d;%lu;%lu;%.9f;%.9f" \
                    % (call[1]["type"], call[1]["comm_idx"], call[1]["group_idx"],
                       tsc_start_mpi, tsc_end_mpi,
                       time_start_mpi, time_end_mpi)
        if gen_info["no_fixed"] == 0:
            mpi_info += ";%lu;%lu;%lu" % (fixed_1_mpi, fixed_2_mpi, fixed_3_mpi)
            if gen_info["no_pmu"] == 0:
                mpi_info += ";%lu;%lu;%lu;%lu" % (pmu_1_mpi, pmu_2_mpi, pmu_3_mpi, pmu_4_mpi)
                if gen_info["HT_enable"] == 0:
                    mpi_info += ";%lu;%lu;%lu;%lu" % (pmu_5_mpi, pmu_6_mpi, pmu_7_mpi, pmu_8_mpi)
        
        # add source and recv network
        dest = ""
        if call[1]["dest"] == MPI_ALLV:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank >= 0:
                    dest += ";%d" % (call[1]["send_size"] * call[1]["send_count"][local_rank])
                else:
                    dest += ";0"
        elif call[1]["dest"] == MPI_ALL:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank >= 0:
                    dest += ";%d" % (call[1]["send_size"] * call[1]["send_count"])
                else:
                    dest += ";0"
        elif call[1]["dest"] == MPI_NONE:
            for j in range(groups[GROUP_WORLD]["size"]):
                dest += ";0"
        else:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank == call[1]["dest"]:
                    dest += ";%d" % (call[1]["send_size"] * call[1]["send_count"])
                else:
                    dest += ";0"
                
        source = ""  
        if call[1]["source"] == MPI_ALLV:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank >= 0:
                    source += ";%d" % (call[1]["recv_size"] * call[1]["recv_count"][local_rank])
                else:
                    source += ";0"
        elif call[1]["source"] == MPI_ALL:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank >= 0:
                    source += ";%d" % (call[1]["recv_size"] * call[1]["recv_count"])
                else:
                    source += ";0"
        elif call[1]["source"] == MPI_NONE:
            for j in range(groups[GROUP_WORLD]["size"]):
                source += ";0"
        else:
            for j in range(groups[GROUP_WORLD]["size"]):
                local_rank = world_rank_2_local_rank(j, groups[call[1]["group_idx"]])
                if local_rank == call[1]["source"]:
                    source += ";%d" % (call[1]["recv_size"] * call[1]["recv_count"])
                else:
                    source += ";0"
                    
        f.write(app_info + mpi_info + dest + source + "\n")

    f.close()

def split_workload(rank, size, num_workload):
    workload_idx = []
    
    idx = 0
    for i in range(num_workload):
        if idx == rank: workload_idx.append(i)
        idx = idx + 1 if (idx+1) < size else 0
        
    return workload_idx

# Main
if __name__ == "__main__":
    # Parsing arguments
    parser = argparse.ArgumentParser(description='Tool to transform PMPI log into CSV file')
    parser.add_argument('-i', '--input-dir', help='Input directory', required=True, type=str)
    parser.add_argument('-o', '--output-dir', help='Output directory', required=False, type=str, default='./')
    parser.add_argument('-nl', '--no-labels', help='Disable labels in CSV file', required=False, action='store_true', default=False)
    args = parser.parse_args()
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    
    input_files = []
    for file_name in os.listdir(args.input_dir):
        pattern = re.compile("pmpi_rank_[0-9]+\.bin$")
        if pattern.match(file_name):
            input_files.append(os.path.join(args.input_dir, file_name))
            
    workload = split_workload(rank, size, len(input_files))

    for i in workload:
        input_file = input_files[i]
        with open(input_file, "rb") as f_input:
            gen_info = get_gen_info(f_input, input_file, args.output_dir, args.no_labels)
            groups = get_groups(f_input, gen_info, input_file, args.output_dir, args.no_labels)
            comms = get_comms(f_input, gen_info, input_file, args.output_dir, args.no_labels)
            get_mpi_calls(f_input, gen_info, groups, comms, input_file, args.output_dir, args.no_labels)