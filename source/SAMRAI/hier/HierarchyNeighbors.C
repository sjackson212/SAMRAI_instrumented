/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2015 Lawrence Livermore National Security, LLC
 * Description:   An AMR hierarchy of patch levels
 *
 ************************************************************************/
#include "SAMRAI/hier/HierarchyNeighbors.h"

#include "SAMRAI/hier/Connector.h"

namespace SAMRAI {
namespace hier {

/*
 **************************************************************************
 * Constructor for HierarchyNeighbors
 **************************************************************************
 */

HierarchyNeighbors::HierarchyNeighbors(
   const PatchHierarchy& hierarchy,
   int coarsest_level,
   int finest_level)
: d_coarsest_level(coarsest_level),
  d_finest_level(finest_level)
{
   int num_levels = hierarchy.getNumberOfLevels();
   TBOX_ASSERT(coarsest_level >= 0);
   TBOX_ASSERT(coarsest_level <= finest_level);
   TBOX_ASSERT(finest_level < num_levels);

   d_same_level_nbrs.resize(num_levels);
   d_finer_level_nbrs.resize(num_levels);
   d_coarser_level_nbrs.resize(num_levels);

   for (int ln = finest_level; ln >= coarsest_level; --ln) {
      const boost::shared_ptr<PatchLevel>& current_level =
         hierarchy.getPatchLevel(ln);

      /*
       * If there is a finer level than ln, find the finer level neighbors
       */
      if (ln != finest_level) {

         boost::shared_ptr<PatchLevel> finer_level(
            hierarchy.getPatchLevel(ln+1));

         const Connector& coarse_to_fine =
            current_level->findConnector(
               *finer_level,
               IntVector::getOne(hierarchy.getDim()),
               CONNECTOR_IMPLICIT_CREATION_RULE,
               true);

         for (PatchLevel::iterator ip(current_level->begin());
              ip != current_level->end(); ++ip) {

            const boost::shared_ptr<Patch>& patch = *ip;
            const Box& box = patch->getBox();
            const BoxId& box_id = box.getBoxId();
            BoxContainer& finer_nbrs = d_finer_level_nbrs[ln][box_id];

            if (coarse_to_fine.hasNeighborSet(box_id)) {
               coarse_to_fine.getNeighborBoxes(box_id, finer_nbrs);
            }
         }
      }

      /*
       * If there is a coarser level than ln, find the coarser level neighbors
       */
      if (ln != coarsest_level) {

         boost::shared_ptr<PatchLevel> coarser_level(
            hierarchy.getPatchLevel(ln-1));

         const Connector& fine_to_coarse =
            current_level->findConnector(
               *coarser_level,
               IntVector::getOne(hierarchy.getDim()),
               CONNECTOR_IMPLICIT_CREATION_RULE,
               true);

         for (PatchLevel::iterator ip(current_level->begin());
              ip != current_level->end(); ++ip) {

            const boost::shared_ptr<Patch>& patch = *ip;
            const Box& box = patch->getBox();
            const BoxId& box_id = box.getBoxId();
            BoxContainer& coarser_nbrs = d_coarser_level_nbrs[ln][box_id];

            if (fine_to_coarse.hasNeighborSet(box_id)) {
               fine_to_coarse.getNeighborBoxes(box_id, coarser_nbrs);
            }
         }
      }

      /*
       * Find the neighbors on the current level
       */

      const Connector& current_to_current =
         current_level->findConnector(
            *current_level,
            IntVector::getOne(hierarchy.getDim()),
            CONNECTOR_IMPLICIT_CREATION_RULE,
            true);

      for (PatchLevel::iterator ip(current_level->begin());
           ip != current_level->end(); ++ip) {
         const boost::shared_ptr<Patch>& patch = *ip;
         const Box& box = patch->getBox();
         const BoxId& box_id = box.getBoxId();
         BoxContainer& same_level_nbrs = d_same_level_nbrs[ln][box_id];

         BoxContainer same_nbr_boxes;
         if (current_to_current.hasNeighborSet(box_id)) {
            current_to_current.getNeighborBoxes(box_id, same_nbr_boxes);
         }
         for (BoxContainer::iterator itr =
              same_nbr_boxes.begin(); itr != same_nbr_boxes.end(); ++itr) {
            if (box_id != itr->getBoxId()) {
               same_level_nbrs.insert(same_level_nbrs.end(), *itr);
            }
         }
      }
   }
}

/*
 **************************************************************************
 * Destructor
 **************************************************************************
 */

HierarchyNeighbors::~HierarchyNeighbors()
{
}

}
}
