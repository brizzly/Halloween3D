/*
 *  * C-Mirror -- dirlib.h
 *   *
 *    * Matti Aarnio <mea@nic.funet.fi> 1995
 *     */

/* UNIX-like in-core directory tree,
 *    EXCEPT this allows ONLY ONE LINK
 *       OF FILE in anywhere at the tree!
 *
 *          For usage-notes see below		*/

#ifndef __
# ifdef __STDC__
#  define __(x) x
# else
#  define __(x) ()
# endif
#endif

typedef enum { DL_FILE = 'f', DL_DIR = 'd', DL_OTHER = 'o' } t_dltype;

typedef struct t_filent {
		t_dltype	type;		/* type (file/dir/other)	*/
			int		mode;		/* UNIX-style mode		*/
				time_t		mtime;		/* Last modify time		*/
					long		size;		/* size (for dir: irrelevant)	*/
						struct t_dirent	*dir;		/* Which dir this file is in	*/
							struct t_dirent *dirval;	/* For DL_DIR: directory info	*/
								struct t_filent	*peer;		/* for file remapping in mirroring */
									char		name[1];	/* Name - sans path!		*/
												/* Alloc sizeof(struct t_filent)+strlen(name) ! */
} t_filent;

	/* When t_filent->type == DL_DIR, content is defined with following
	 * 	   structure to which t_filent->dir  points to.
	 * 	   	   For other type of files the same pointer reports the LOCATION
	 * 	   	   	   of the file -- what dir it is in. */

struct t_dirent {
		t_filent	*dirfil;	/* Directory's own file-entry	*/
			t_filent	*parent;	/* parent of this dir		*/
				int		filecnt;	/* How many entries in it	*/
					int		filespc;	/* How much allocated space	*/
						t_filent	**files;	/* Array of entries		*/
							char		path[1];	/* Name of the directory	*/
										/* Alloc sizeof(struct t_dirent)+strlen(path) ! */
};

extern t_filent *dl_createroot __((void));
extern void	 dl_free   __((t_filent *root));
extern t_filent *dl_newent __((t_filent *root, char *path, t_dltype type));
extern t_filent *dl_find   __((t_filent *root, char *path));
extern t_filent *dl_delete __((t_filent *entry));

extern void      dl_namesort __((t_filent *root));
extern int	 dl_namecompare __((const void *, const void*)); /* For QSORT */
extern int	 dl_revtimecompare __((const void *, const void*));
extern int	 dl_timecompare __((const void *, const void*));

extern t_filent *dl_yield_dirs __((t_filent*root, void **state, int startflg));
extern t_filent *dl_yield_all  __((t_filent*root, void **state, int startflg));
extern t_filent *dl_yield_dirs_depth __((t_filent *root, void **state,
								 int startflg));

/*
 *  *	Usage notes:
 *   *
 *    *	FILE CAN EXIST ONLY AT ONE PLACE IN THE TREE!
 *     *
 *      *	Start with   dl_createroot()  - its output is a root for a tree.
 *       *
 *        *	Trash whole (sub)tree with  dl_free(root)
 *         *
 *          *	Insert entries into the tree with  dl_newent(root,path,type).
 *           *	It returns NULL for error, else pointer to THE NEW ELEMENT.
 *            *	You can speed-up the insert by remembering the DIRECTORY where
 *             *	the entries go, and using it as `root' for the new entries,
 *              *	instread of using full paths for them.
 *               *
 *                *	Locate entries from a tree with  dl_find(root,path), it returns
 *                 *	pointer to the named entry.
 *                  *
 *                   *	Delete entry from the tree with   dl_delete(entry).  It returns
 *                    *	non-null if the deletion was ok. Root-entry or non-empty dir is
 *                     *	not deleted!
 *                      *
 *                       *	Alphasort (recursive, with current collate-sequence) the subtree
 *                        *	with  dl_namesort(root).
 *                         *
 *                          *	Iterators for traversals of the tree are:
 *                           *
 *                            *		void *iterstate;
 *                             *		t_filent *fep;
 *                              *		for(fep = dl_yield_XXX(root,&iterstate,1) ;
 *                               *		    fep != NULL ;
 *                                *		    fep = dl_yield_XXX(root,&iterstate,0)) {
 *                                 *			...
 *                                  *		}
 *                                   *
 *                                    *	dl_yield_all() iterates in post-order (that is, recurse after
 *                                     *			yielding the directory) all entries of the tree.
 *                                      *	dl_yield_dirs() twin of  dl_yield_all() with "dirs only" filter.
 *                                       *	dl_yield_dirs_depth() iterates pre-order (recurse directories
 *                                        *			before yielding them) all entries with "dirs
 *                                         *			only" filter.
 *                                          *
 *                                           *
 *                                            *	After using   dl_namesort(root), the  dl_yield_all(root,...)  yields
 *                                             *	all names in pure alpha-order -- including directory paths.
 *                                              */

