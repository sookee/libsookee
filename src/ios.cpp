/*
 * logrep.cpp
 *
 *  Created on: 3 Aug 2011
 *      Author: oasookee@gmail.com
 */

/*-----------------------------------------------------------------.
| Copyright (C) 2011 SooKee oasookee@gmail.com               |
'------------------------------------------------------------------'

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

http://www.gnu.org/licenses/gpl-2.0.html

'-----------------------------------------------------------------*/

#include <sookee/ios.h>

#include <dirent.h>
#include <sys/stat.h>

#include <sookee/types.h>

namespace sookee { namespace ios {

using namespace sookee::types;

str expand_env(const str& var, int flags)
{
	str exp = var;
	wordexp_t p;
	if(!wordexp(var.c_str(), &p, flags))
	{
		if(p.we_wordc && p.we_wordv[0])
			exp = p.we_wordv[0];
		wordfree(&p);
	}
	return exp;
}

std::istream& getstring(std::istream& is, str& s)
{
	char term = ' ';
	if((is >> std::ws).peek() == '"' || is.peek() == '\'')
		is.get(term);

	char c;
	str n;
	while(is.get(c) && c != term)
		n.append(1, c);

	if(n.empty())
		is.setstate(std::ios::failbit);
	else
	{
		s = n;
		is.clear();
	}

	return is;
}

int ls(const str& folder, str_vec &files)
{
    DIR* dir;
	dirent* dirp;

	if(!(dir = opendir(folder.c_str())))
		return errno;

	files.clear();
	while((dirp = readdir(dir)))
		files.push_back(dirp->d_name);

	if(closedir(dir))
		return errno;

	return 0;
}

bool ls(const str& folder, str_vec& folders, str_vec &files)
{
    DIR* dir;
	dirent* dirp;

	if(!(dir = opendir(folder.c_str())))
		return false;

	files.clear();
	while((dirp = readdir(dir)))
	{
		if(dirp->d_type == DT_DIR)
			folders.push_back(dirp->d_name);
		else if(dirp->d_type == DT_REG)
			files.push_back(dirp->d_name);
		else if(dirp->d_type == DT_UNKNOWN)
		{
			struct stat s;
			if(!stat(dirp->d_name, &s))
			{
				if(S_ISDIR(s.st_mode))
					folders.push_back(dirp->d_name);
				else if(S_ISREG(s.st_mode))
					files.push_back(dirp->d_name);
			}
		}
	}
	if(closedir(dir))
		return false;

	return true;
}

str_vec ls(const str& folder, ftype t)
{
    DIR* dir;
	dirent* dirp;
	str_vec files;

	if(!(dir = opendir(folder.c_str())))
		return files;

	struct stat s;

	while((dirp = readdir(dir)))
	{
		if(!stat(dirp->d_name, &s))
		{
			if((t == ftype::dir && S_ISDIR(s.st_mode))
			|| (t == ftype::reg && S_ISREG(s.st_mode))
			|| (t == ftype::chr && S_ISCHR(s.st_mode))
			|| (t == ftype::blk && S_ISBLK(s.st_mode))
			|| (t == ftype::fifo && S_ISFIFO(s.st_mode))
			|| (t == ftype::lnk && S_ISLNK(s.st_mode))
			|| (t == ftype::sock && S_ISSOCK(s.st_mode))
			|| (t == ftype::all))
				files.push_back(dirp->d_name);
		}
	}

	closedir(dir);

	return files;
}

//template<typename... Args>
//bool Fork::exec(const str& dir, const str& prog, Args... args)
//{
//	pid_t pid;
//
//	int pipe_w[2]; // write to child
//	int pipe_r[2]; // read from child
//
//	if(pipe(pipe_w) || pipe(pipe_r))
//		return log_report(strerror(errno));
//
//	if((pid = fork()) == -1)
//		return log_report(strerror(errno));
//
//	if(pid)
//	{
//		// parent
//		stdip.reset(new(std::nothrow) stdiostream(pipe_r[0], std::ios::in));
//		stdop.reset(new(std::nothrow) stdiostream(pipe_w[1], std::ios::out));
//
//		close(pipe_r[1]);
//		close(pipe_w[0]);
//
//		if(!stdip.get() || !stdop.get())
//			return log_report("Unable to create stdiostream object.");
//
//		return true;
//	}
//	else
//	{
//		// child
//
////			rlimit lim;
////			if(!getrlimit(RLIMIT_NPROC, &lim))
////			{
////				lim.rlim_max += 10;
////				lim.rlim_cur += 10;
////				setrlimit(RLIMIT_NPROC, &lim);
////			}
//
//		close(1);
//		dup(pipe_r[1]); // lowest unused fd
//
//		close(0);
//		dup(pipe_w[0]); // lowest unused fd
//
//		close(pipe_r[0]);
//		close(pipe_r[1]);
//		close(pipe_w[0]);
//		close(pipe_w[1]);
//
//		str prog_name = expand_env(dir + "/" + prog, WRDE_SHOWERR);
//
//		if(!dir.empty())
//			chdir(dir.c_str());
//
//		execlp(prog_name.c_str(), prog_name.c_str(), args..., 0);
//
//		log("ERROR: " << strerror(errno));
//		return false; // execl() failed
//	}
//
//	return true;
//}

}} // sookee::ios
