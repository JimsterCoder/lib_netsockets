/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
 
/* <DESC>
 * Performs an FTP upload and renames the file just after a successful
 * transfer.
 * </DESC>
 */
 
#define LOCAL_FILE      "rubbish.txt"
#define REMOTE_URL      "ftp://ftp.jstulen.com"
 
// static size_t read_callback(char *ptr, size_t size, size_t nmemb, FILE *stream)
// {
//   unsigned long nread;
//   /* in real-world cases, this would probably get this data differently
//      as this fread() stuff is exactly what the library already would do
//      by default internally */
//   size_t retcode = fread(ptr, size, nmemb, stream);
 
//   if(retcode > 0) {
//     nread = (unsigned long)retcode;
//     fprintf(stderr, "*** We read %lu bytes from file\n", nread);
//   }
 
//   return retcode;
// }
 
int main(void)
{
  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  struct stat file_info;
  unsigned long fsize;
 
//   struct curl_slist *headerlist = NULL;
 
  /* get the file size of the local file */
  if(stat(LOCAL_FILE, &file_info)) {
    printf("Couldn't open '%s': %s\n", LOCAL_FILE, strerror(errno));
    return 1;
  }
  fsize = (unsigned long)file_info.st_size;
 
  printf("Local file size: %lu bytes.\n", fsize);
 
  /* get a FILE * of the same file */
  hd_src = fopen(LOCAL_FILE, "rb");
 
  /* Init curl
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
   
    /* enable uploading */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_APPEND, 1L);
 
    /* specify target */
    curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp.jstulen.com/uploadedfile3.txt");
    curl_easy_setopt(curl, CURLOPT_USERNAME, "ftpmonkeylab");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "FTP-user1");
 
    /* now specify which the pointer to the file to upload */
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, fsize);
 
    /* Now run off and do what you have been told! */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */
 
  curl_global_cleanup();
}
