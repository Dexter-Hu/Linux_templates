cmake_minimum_required(VERSION 3.0.0) #规定了cmake的最低版本号

set(POSIX_SHM_CREATE ${PROJECT_SOURCE_DIR}/posix_shm_create)
set(POSIX_SHM_WRITE  ${PROJECT_SOURCE_DIR}/posix_shm_write)
set(POSIX_SHM_READ   ${PROJECT_SOURCE_DIR}/posix_shm_read)
set(POSIX_SHM_UNLINK ${PROJECT_SOURCE_DIR}/posix_shm_unlink)
subdirs(POSIX_SHM_CREATE POSIX_SHM_WRITE POSIX_SHM_READ POSIX_SHM_UNLINK)