#ifndef GPU_GROUP_H
#define GPU_GROUP_H

#include "gpu.h"

/* A group of array references in a kernel that should be handled together.
 * If private_tile is not NULL, then it is mapped to registers.
 * Otherwise, if shared_tile is not NULL, it is mapped to shared memory.
 * Otherwise, it is accessed from global memory.
 */
struct gpu_array_ref_group {
	/* The references in this group access this local array. */
	struct gpu_local_array_info *local_array;
	/* This is the corresponding array. */
	struct gpu_array_info *array;
	/* Position of this group in the list of reference groups of array. */
	int nr;

	/* The following fields are use during the construction of the groups.
	 * access is the combined access relation relative to the shared
	 * memory tiling.  In particular, the domain of the map corresponds
	 * to the first shared_len dimensions of the computed schedule.
	 * write is set if any access in the group is a write.
	 * exact_write is set if all writes are definite writes.
	 * slice is set if there is at least one access in the group
	 * that refers to more than one element
	 */
	isl_map *access;
	int write;
	int exact_write;
	int slice;

	/* The shared memory tile, NULL if none. */
	struct gpu_array_tile *shared_tile;

	/* The private memory tile, NULL if none. */
	struct gpu_array_tile *private_tile;

	/* References in this group; point to elements of a linked list. */
	int n_ref;
	struct gpu_stmt_access **refs;

	/* Last shared memory tile dimension that affects tile of this group. */
	int last_shared;
};

int gpu_group_references(struct gpu_gen *gen);

__isl_give isl_printer *gpu_array_ref_group_print_name(
	struct gpu_array_ref_group *group, __isl_take isl_printer *p);
void gpu_array_ref_group_compute_tiling(struct gpu_array_ref_group *group);
__isl_give isl_union_map *gpu_array_ref_group_access_relation(
	struct gpu_array_ref_group *group, int read, int write);
struct gpu_array_ref_group *gpu_array_ref_group_free(
	struct gpu_array_ref_group *group);

#endif