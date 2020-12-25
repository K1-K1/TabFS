//usr/bin/env cc -o test "$0" && ./test; exit

// (run this file directly with `./test.c` in most shells; if that
// doesn't work, run it with `sh test.c`)

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <assert.h>
#include <wordexp.h>

int file_contents_equal(char* path, char* contents) {
    // hehe: https://twitter.com/ianh_/status/1340450349065244675
    setenv("path", path, 1);
    setenv("contents", contents, 1);
    return system("[ \"$contents\" == \"$(cat \"$path\")\" ]") == 0;
}

char* expand(char* phrase) { // expand path with wildcard
    wordexp_t result; assert(wordexp(phrase, &result, 0) == 0);
    return result.we_wordv[0];
}

// integration tests
int main() {
    assert(system("node extension/background.js") == 0); // run quick local JS tests

    // reload the extension so we know it's the latest code.
    system("echo reload > fs/mnt/runtime/reload"); // this may error, but it should still have effect
    // FIXME: race here
    sleep(4);

    // FIXME: synthesize some kind of web page
    assert(system("echo about:blank > fs/mnt/tabs/create") == 0);
    // FIXME: race here
    assert(file_contents_equal("fs/mnt/tabs/last-focused/url.txt", "about:blank"));
    assert(system("echo remove > fs/mnt/tabs/last-focused/control") == 0);

    assert(file_contents_equal(expand("fs/mnt/extensions/TabFS*/enabled"), "true"));

    assert(1); printf("Done!\n"); 
}
