# sgx-wallet
[![license](https://img.shields.io/badge/license-GPL3-brightgreen.svg)](https://github.com/asonnino/sgx-wallet/blob/master/LICENSE)

This is a simple password wallet application based on Intel SGX on linux. 


## Pre-requisites
Ensure to have installed the Intel SGX Linux [drivers](https://github.com/intel/linux-sgx-driver) and [SDK](https://github.com/intel/linux-sgx).

## Install
First, source the Intel SGX SDK as described [here](https://github.com/intel/linux-sgx#install-the-intelr-sgx-sdk-1):
```
$ source ${sgx-sdk-install-path}/environment
```
where `${sgx-sdk-install-path}` is your SDK installation path. Then, clone and build the source code:
```
$ git clone https://github.com/asonnino/sgx-wallet.git
$ cd sgx-wallet
$ make
```

## Usage
Basic usage is as follows: 
```
sgx-wallet [-h Show this screen] [-v Show version] [-t Run tests] [-n master-password] [-p master-password -c new-master-password] [-p master-password -a -x items_title -y items_username -z toitems_password] [-p master-password -r items_index]
```
or specifically:
  - `sgx-wallet -h`: show help;
  - `sgx-wallet -v`: show version;
  - `sgx-wallet -t`: run tests;
  - `sgx-wallet -n master-password`: create a new wallet with `<master-password <master-passowrd>`;
  - `sgx-wallet -p master-password -c new-master-password`: Change current master-password to `<new-master-password>`;
  - `sgx-wallet -p master-password -a -x items_title -y items_username -z toitems_password`:  Add a new item to the wallet with title `<items_title>`, username `<items_username>`, and password `<items_password>`;
  - `sgx-wallet -p master-password -r items_index`: Remove item at index `<items_index>` from the wallet.

## Contribute
Any help is welcome through PRs!

## License
[The GPLv3 license](https://www.gnu.org/licenses/gpl-3.0.en.html)