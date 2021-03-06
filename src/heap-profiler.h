// Copyright 2009-2010 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_HEAP_PROFILER_H_
#define V8_HEAP_PROFILER_H_

#include "heap-snapshot-generator-inl.h"
#include "isolate.h"
#include "smart-pointers.h"

namespace v8 {
namespace internal {

class HeapSnapshot;

class HeapProfiler {
 public:
  explicit HeapProfiler(Heap* heap);
  ~HeapProfiler();

  size_t GetMemorySizeUsedByProfiler();

  HeapSnapshot* TakeSnapshot(
      const char* name,
      v8::ActivityControl* control,
      v8::HeapProfiler::ObjectNameResolver* resolver);
  HeapSnapshot* TakeSnapshot(
      String* name,
      v8::ActivityControl* control,
      v8::HeapProfiler::ObjectNameResolver* resolver);

  void StartHeapObjectsTracking(bool track_allocations);
  void StopHeapObjectsTracking();
  AllocationTracker* allocation_tracker() { return *allocation_tracker_; }
  HeapObjectsMap* heap_object_map() { return *ids_; }
  StringsStorage* names() { return *names_; }

  SnapshotObjectId PushHeapObjectsStats(OutputStream* stream);
  int GetSnapshotsCount();
  HeapSnapshot* GetSnapshot(int index);
  SnapshotObjectId GetSnapshotObjectId(Handle<Object> obj);
  void DeleteAllSnapshots();
  void RemoveSnapshot(HeapSnapshot* snapshot);

  void ObjectMoveEvent(Address from, Address to, int size);

  void AllocationEvent(Address addr, int size);

  void UpdateObjectSizeEvent(Address addr, int size);

  void DefineWrapperClass(
      uint16_t class_id, v8::HeapProfiler::WrapperInfoCallback callback);

  v8::RetainedObjectInfo* ExecuteWrapperClassCallback(uint16_t class_id,
                                                      Object** wrapper);
  void SetRetainedObjectInfo(UniqueId id, RetainedObjectInfo* info);

  bool is_tracking_object_moves() const { return is_tracking_object_moves_; }
  bool is_tracking_allocations() { return !allocation_tracker_.is_empty(); }

  Handle<HeapObject> FindHeapObjectById(SnapshotObjectId id);

 private:
  Heap* heap() const { return ids_->heap(); }

  // Mapping from HeapObject addresses to objects' uids.
  SmartPointer<HeapObjectsMap> ids_;
  List<HeapSnapshot*> snapshots_;
  SmartPointer<StringsStorage> names_;
  unsigned next_snapshot_uid_;
  List<v8::HeapProfiler::WrapperInfoCallback> wrapper_callbacks_;
  SmartPointer<AllocationTracker> allocation_tracker_;
  bool is_tracking_object_moves_;
};

} }  // namespace v8::internal

#endif  // V8_HEAP_PROFILER_H_
