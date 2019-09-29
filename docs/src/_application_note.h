/**
 * \page            page_appnote Application note
 * \tableofcontents
 *
 * \section         sect_getting_started Getting started
 *
 * Library development is fully hosted on Github and there is no future plans to move to any other platform.
 *
 * There are `2` repositories
 *
 *	- <a href="https://github.com/MaJerle/GPS_NMEA_Parser"><b>GPS_NMEA_Parser</b></a>: Source code of library itself.
 *	    - Repository is required when developing final project
 *
 *	- <a href="https://github.com/MaJerle/GPS_NMEA_Parser_res"><b>GPS_NMEA_Parser_res</b></a>: Resources, development code,
 *		documentation sources, examples, code snippets, etc.
 *      - This repository uses `GPS_NMEA_Parser` repository as `submodule`
 *	    - Repository is used to evaluate library using prepared examples
 *
 * \subsection      sect_clone_res Clone resources with examples
 *
 * Easiest way to test the library is to clone resources repository.
 *
 * \par             First-time clone
 *
 *  - Download and install `git` if not already
 *  - Open console and navigate to path in the system to clone repository to. Use command `cd your_path`
 *  - Run `git clone --recurse-submodules https://github.com/MaJerle/GPS_NMEA_Parser_res` command to clone repository including submodules
 *  - Navigate to `examples` directory and run favourite example
 *
 * \par             Already cloned, update to latest version
 *
 *  - Open console and navigate to path in the system where your resources repository is. Use command `cd your_path`
 *  - Run `git pull origin master --recurse-submodules` command to pull latest changes and to fetch latest changes from submodules
 *  - Run `git submodule foreach git pull origin master` to update & merge all submodules
 *
 * \subsection      sect_clone_lib Clone library only
 *
 * If you are already familiar with library and you wish to include it in existing project, easiest way to do so is to clone library repository only.
 *
 * \par             First-time clone
 *
 *  - Download and install `git` if not already
 *  - Open console and navigate to path in the system to clone repository to. Use command `cd your_path`
 *  - Run `git clone --recurse-submodules https://github.com/MaJerle/GPS_NMEA_Parser` command to clone repository
 * 
 * \par             Already cloned, update to latest version
 *
 *  - Open console and navigate to path in the system where your repository is. Use command `cd your_path`
 *  - Run `git pull origin master --recurse-submodules` to update & merge latest repository changes
 */